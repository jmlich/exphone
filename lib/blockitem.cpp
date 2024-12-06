#include <QDebug>
#include "blockitem.h"

BlockItem::BlockItem(QString &number, QString &name, QString &note, QDateTime &lastSeen, bool blocked, int count)
    : m_number(number), m_name(name), m_note(note), m_lastSeen(lastSeen), m_blocked(blocked), m_count(count)
{
}
