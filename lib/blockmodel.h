#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QtSql>
#include "blockitem.h"

#include <QAbstractListModel>

#define DB_FILE "blockmodel.db"

class BlockModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BlockModel(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void addItem(const QString& number, const QString& name, const QString& note, bool blocked);
    Q_INVOKABLE void setBlocked(const QString& number, bool blocked);
    Q_INVOKABLE void removeItem(const QString& number);
    Q_INVOKABLE bool isBlocked(QString& number);

    void loadAll();

    void initDb();

private:
    QList<BlockItem> m_blocks;

    enum BlockModelRoles {
        NumberRole = Qt::UserRole + 1,
        NameRole,
        NoteRole,
        LastSeenRole,
        BlockedRole,
        CountRole
    };

signals:


};

#endif // BLOCKMODEL_H