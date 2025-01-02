/* Copyright (C) 2017 Dan Chapman <dpniel@ubuntu.com>

   This file is part of Dekko email client for Ubuntu devices

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PUSHCLIENT_H
#define PUSHCLIENT_H

#include <QObject>
#include <QDBusConnection>
#include <QJsonObject>

class PushClient : public QObject
{
    Q_OBJECT
public:
    explicit PushClient(QObject *parent = 0);

    static PushClient *instance();

    // Send a notification
    bool send(const QJsonObject &message);
    // Update a notification
    bool update(const QString &tag, const QJsonObject &message);

    bool hasTag(const QString &tag);
    bool clearPersistent(const QString &tag);
    bool updateCount(const QString &tag = QString(), const bool remove = false);

private:
    QByteArray makePath(const QString &appId);
    QStringList getPersistent();

    QDBusConnection m_conn;
    QStringList m_tags;
};

#endif // PUSHCLIENT_H
