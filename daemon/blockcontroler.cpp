#include "blockcontroler.h"
#include "exphoneconfig.h"
#include <QObject>

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
        qDebug() << "TODO send notification";
    }
}
