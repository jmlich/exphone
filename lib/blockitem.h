#ifndef BLOCKITEM__H
#define BLOCKITEM__H

#include <QtCore>

class BlockItem
{
public:
    BlockItem(QString &number, QString &name, QString &note, QDateTime &lastSeen, bool blocked, int count);

    QString number() const { return m_number; }
    QString name() const { return m_name; }
    QString note() const { return m_note; }
    QDateTime lastSeen() const { return m_lastSeen; }
    bool blocked() const { return m_blocked; }
    int count() const { return m_count; }

private:
    QString m_number;
    QString m_name;
    QString m_note;
    QDateTime m_lastSeen;
    int m_count;
    bool m_blocked;
};

#endif // BLOCKITEM_H
