#include "blockmodel.h"
#include <QtSql>
#include <QDir>

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

QVariantMap BlockModel::get(int row) const {
    if (row < 0 || row >= m_blocks.size()) {
        return QVariantMap();
    }

    QVariantMap data;

    data["number"] = m_blocks.at(row).number();
    data["name"] = m_blocks.at(row).name();
    data["note"] = m_blocks.at(row).note();
    data["lastSeen"] = m_blocks.at(row).lastSeen();
    data["blocked"] = m_blocks.at(row).blocked();
    data["count"] = m_blocks.at(row).count();

    return data;
}


void BlockModel::initDb() {
    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    if (!appDataDir.exists()) {
        appDataDir.mkpath(".");
    }

    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qFatal("SQLite driver not available");
    }

    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString appDataDirFilePath = appDataDir.filePath(DB_FILE);
    qDebug() << "Loading database from" << appDataDirFilePath;
    m_db.setDatabaseName(appDataDirFilePath);

    if (!m_db.open()) {
        qFatal("Failed to open database: %s", qPrintable(m_db.lastError().text()));
        exit(1);
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS blocks ("
               "number VARCHAR(32) PRIMARY KEY NOT NULL,"
               "name VARCHAR(32) DEFAULT '',"
               "note VARCHAR(32) DEFAULT '',"
               "lastSeen DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "blocked BOOLEAN DEFAULT 0,"
               "count INT DEFAULT 0)");

    deleteOldEntries();
}

void BlockModel::loadAll() {

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
    emit rowCountChanged();

    qDebug() << Q_FUNC_INFO << m_blocks.count();

}


void BlockModel::deleteOldEntries() {
    QSqlQuery query;

    query.prepare("DELETE FROM blocks WHERE lastSeen < DATETIME('now', '-1 month') AND blocked = 0");

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to delete old entries from blocks table:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully deleted old entries from blocks table.";
    }
}


void BlockModel::addItem(const QString& number, const QString& name, const QString& note, bool blocked) {

    QSqlQuery query;
    query.prepare("INSERT INTO blocks (number, name, note, count, blocked) "
                  "VALUES (:number, :name, :note, :count, :blocked)");

    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":note", note);
    query.bindValue(":count", 0);
    query.bindValue(":blocked", blocked);

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to insert into blocks table:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully added item to blocks table:" << number << name;
    }

    loadAll();

}

void BlockModel::updateItem(const QString& oldnumber, const QString& number, const QString& name, const QString& note, bool blocked) {

    QSqlQuery query;
    query.prepare("UPDATE blocks SET number = :number, name = :name, note = :note, blocked = :blocked WHERE number = :oldnumber");

    query.bindValue(":oldnumber", oldnumber);
    query.bindValue(":number", number);
    query.bindValue(":name", name);
    query.bindValue(":note", note);
    query.bindValue(":blocked", blocked);

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to update record in blocks table:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully updated item in blocks table:" << number << name;
    }

    loadAll();

}


void BlockModel::logCall(const QString& number, const QString& name) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO blocks (number, name, lastSeen, blocked)
        VALUES (:number, :name, CURRENT_TIMESTAMP, 0)
        ON CONFLICT(number) DO UPDATE SET
            lastSeen = CURRENT_TIMESTAMP,
            count = count + 1
        WHERE number = :number;
    )");
    query.bindValue(":number", number);
    query.bindValue(":name", name);

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to upsert into blocks table:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully upserted item into blocks table:" << number << name;
    }

    loadAll();
}


void BlockModel::setBlocked(const QString& number, bool blocked) {

    QSqlQuery query;
    query.prepare("UPDATE blocks SET blocked = :blocked WHERE number = :number");
    query.bindValue(":blocked", blocked);
    query.bindValue(":number", number);

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to update blocked settings:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully set:" << number << " blocked "<< blocked;
    }

    loadAll();
}

void BlockModel::removeItem(const QString& number) {
    QSqlQuery query;
    query.prepare("DELETE FROM blocks WHERE number = :number");
    query.bindValue(":number", number);

    if (!query.exec()) {
        qWarning() << Q_FUNC_INFO << "Failed to delete from blocks table:" << query.lastError().text();
    } else {
        qDebug() << Q_FUNC_INFO << "Successfully removed item from blocks table:" << number;
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

