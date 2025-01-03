#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

#include "blockcontroler.h"
#include "exphoneconfig.h"
#include "pushclient.h"


BlockControler::BlockControler(QObject *parent) :
    QObject(parent) {

    m_blockModel.initDb();

#if defined(MER_EDITION_SAILFISH) || defined(UUITK_EDITION)
    connect(&m_voiceCallController, &watchfish::VoiceCallController::ringingChanged, this, &BlockControler::onRingingChanged);
#endif
}

void BlockControler::onRingingChanged() {
#if defined(MER_EDITION_SAILFISH) || defined(UUITK_EDITION)
    qDebug() << Q_FUNC_INFO << m_voiceCallController.ringing();

    if (m_voiceCallController.ringing()) {
        QString callerId = m_voiceCallController.callerId();
        QString callerName = m_voiceCallController.findPersonByNumber(callerId);
        incomingCall(callerId, callerName);
    }
#endif
}

void BlockControler::incomingCall(const QString &_callerId, const QString &callerName)
{
    QString callerId = _callerId;
    bool blocked = false;
    qDebug() << Q_FUNC_INFO << callerId << callerName << blocked;

    bool isHiddenNumber = (callerId == "x-ofono-unknown");
    if (isHiddenNumber && ExphoneConfig::instance()->blockingCLIR()) {
        blocked = true;
    }

    bool isUnknown = (callerId == callerName);
    if (isUnknown && ExphoneConfig::instance()->blockingUnknown()) {
        blocked = true;
    }

    if (!blocked) {
        blocked = m_blockModel.isBlocked(callerId);
    }

    m_blockModel.logCall(callerId, callerName);

    if (blocked) {
#if defined(MER_EDITION_SAILFISH) || defined(UUITK_EDITION)
        m_voiceCallController.hangup();
#endif
        showNotification(callerId, callerName);
    }
}


void BlockControler::showNotification(const QString &callerId, const QString &callerName) {
#if defined(UUITK_EDITION)
    QString summary = QObject::tr("Phone call was blocked");
    QString body;
    if ((callerName == "") || (callerId == callerName)) {
        //: phone number in notification body formatted as "%s"
        body = tr("%1").arg(callerId);
    } else {
        //: phone number and contact name formatted as "%s (%s)"
        body = tr("%1 (%2)").arg(callerId, callerName);
    }

    QString icon = QString("file://%1/../share/icons/hicolor/512x512/apps/exphone.png").arg(QCoreApplication::applicationDirPath());

    QJsonObject c;
    c["summary"] = summary;
    c["body"] = body;
    c["popup"] = true;
    c["persist"] = true;
    c["icon"] = icon;
//    QJsonArray actions = QJsonArray();
//    QString actionUri = QStringLiteral("appid://com.github.jmlich.exphone/exphone/current-user-version");
//    actions.append(actionUri);
//    c["actions"] = actions;

    QJsonObject notification;
    notification["card"] = c;
    notification["sound"] = true;
    notification["vibrate"] = false;
    QJsonObject message;
    message["notification"] = notification;

    PushClient *pushClient = PushClient::instance();
    if (pushClient->send(message)) {
        qDebug() << Q_FUNC_INFO << "Message sent successfully." << summary;
    } else {
        qWarning() << Q_FUNC_INFO << "Failed to send the message." << summary;
    }
#endif
}