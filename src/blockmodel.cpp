#include "blockmodel.h"
#include <QtSql>

BlockModel::BlockModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

QHash<int, QByteArray> BlockModel::roleNames() const  {
    QHash<int, QByteArray> roles;
    roles[NumberRole] = "number";
    roles[NameRole] = "name";
    roles[NoteRole] = "note";
    roles[LastSeenRole] = "lastSeen";
    roles[BlockedRole] = "blocked";
    roles[CountRole] = "count";
    return roles;
}

QVariant BlockModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() > m_blocks.count()) {
        return QVariant();
    }

    const BlockItem &item = m_blocks[index.row()];
    switch (role) {
    case NumberRole:
        return item.number();
    case NameRole:
        return item.name();
    case NoteRole:
        return item.note();
    case LastSeenRole:
        return item.lastSeen();
    case BlockedRole:
        return item.blocked();
    case CountRole:
        return item.count();
    }
    return QVariant();
}

int BlockModel::rowCount(const QModelIndex &) const{
    return m_blocks.count();
}

void BlockModel::initDb() {
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qFatal("SQLite driver not available");
    }

    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(DB_FILE);

    if (!m_db.open()) {
        qFatal("Failed to open database: %s", qPrintable(m_db.lastError().text()));
        exit(1);
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS blocks ("
               "number VARCHAR(32) PRIMARY KEY NOT NULL,"
               "name VARCHAR(32) NOT NULL,"
               "note VARCHAR(32) NOT NULL,"
               "lastSeen DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "blocked BOOLEAN DEFAULT 0,"
               "count INT)");
}

void BlockModel::loadAll() {
    qDebug() << Q_FUNC_INFO;

    beginResetModel();
    m_blocks.clear();

    QSqlQuery query("SELECT number, name, note, lastSeen, blocked, count FROM blocks ORDER BY name ASC");

    while (query.next()) {
        QString number = query.value(0).toString();
        QString name = query.value(1).toString();
        QString note = query.value(2).toString();
        QDateTime lastSeen = query.value(3).toDateTime();
        bool blocked = query.value(4).toBool();
        int count = query.value(5).toInt();
        m_blocks.append(BlockItem(number, name, note, lastSeen, blocked, count));
    }
    endResetModel();

}

void BlockModel::addItem(const QString& number, const QString& name, const QString& note, bool blocked) {

    QSqlQuery query;
    query.prepare("INSERT INTO blocks (number, name, note, count, lastSeen, blocked) "
                  "VALUES (:number, :name, :note, :count, :lastSeen, :blocked)");

    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":note", note);
    query.bindValue(":lastSeen", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":blocked", blocked);
    query.bindValue(":count", 0);

    if (!query.exec()) {
        qWarning() << "Failed to insert into blocks table:" << query.lastError().text();
    } else {
        qDebug() << "Successfully added item to blocks table:" << number << name;
    }

    loadAll();

}

void BlockModel::setBlocked(const QString& number, bool blocked) {
    qDebug() << Q_FUNC_INFO << number << blocked;

    QSqlQuery query;
    query.prepare("UPDATE blocks SET blocked = :blocked WHERE number = :number");
    query.bindValue(":blocked", blocked);
    query.bindValue(":number", number);

    if (!query.exec()) {
        qWarning() << "Failed to insert into blocks table:" << query.lastError().text();
    } else {
        qDebug() << "Successfully added item to blocks table:" << number;
    }

    loadAll();
}

void BlockModel::removeItem(const QString& number) {
    QSqlQuery query;
    query.prepare("DELETE FROM blocks WHERE number = :number");
    query.bindValue(":number", number);

    if (!query.exec()) {
        qWarning() << "Failed to insert into blocks table:" << query.lastError().text();
    } else {
        qDebug() << "Successfully added item to blocks table:" << number;
    }

    loadAll();
}



bool BlockModel::isBlocked(QString& _number) {
    QSqlQuery query;
    query.prepare("SELECT blocked, number, name, note, count FROM blocks WHERE number = :number");
    query.bindValue(":number", _number);
    query.exec();
    while (query.next()) {
        bool blocked = query.value(0).toBool();
        qDebug() << Q_FUNC_INFO << "found" << _number << blocked;
        return blocked;
    }
    qDebug() << Q_FUNC_INFO << "not found" << _number;

    return false;
}

