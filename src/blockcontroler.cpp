#include "blockcontroler.h"
#include <QObject>

BlockControler::BlockControler(QObject *parent) :
    QObject(parent) {

    m_blockModel.initDb();

    connect(&m_voiceCallController, &watchfish::VoiceCallController::ringingChanged, this, &BlockControler::onRingingChanged);

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
    bool blocked = m_blockModel.isBlocked(callerId);
    qDebug() << Q_FUNC_INFO << callerId << callerName << blocked;
    if (blocked) {
        m_voiceCallController.hangup();
    }
}
