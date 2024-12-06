#ifndef BLOCKCONTROLER__H
#define BLOCKCONTROLER__H

#include <QObject>

#include "../libwatchfish/voicecallcontroller.h"
#include "blockmodel.h"

class BlockControler: public QObject {
    Q_OBJECT

public:
    explicit BlockControler(QObject *parent = 0);;

private:

    BlockModel m_blockModel;

#if defined(MER_EDITION_SAILFISH) || defined(UUITK_EDITION)
    watchfish::VoiceCallController m_voiceCallController;
#endif

    void incomingCall(const QString &callerId, const QString &callerName);

public slots:
    void onRingingChanged();

};

#endif // BLOCKCONTROLER__H
