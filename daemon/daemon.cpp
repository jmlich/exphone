/*
 * (C) 2014 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 *
 * Main, Daemon functions
 *
 */

#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QtCore/QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusInterface>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "blockcontroler.h"

static void daemonize();
static void signalHandler(int sig);

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit();
#ifdef QT_MESSAGELOGCONTEXT
    fprintf(stderr,"%s %s:%d: %s\n", time, context.file, context.line, localMsg.constData());
#else
    fprintf(stderr, "%s : %s\n", time, localMsg.constData());
#endif
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(myMessageOutput);
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("exphone");

    daemonize();

    setlinebuf(stdout);
    setlinebuf(stderr);

    qDebug() << "Starting app";


    BlockControler blocker;


    return app.exec();
}

static void daemonize()
{
    /* Change the file mode mask */
    umask(0);

    /* Change the current working directory */
    if ((chdir("/tmp")) < 0)
        exit(EXIT_FAILURE);

    /* register signals to monitor / ignore */
    signal(SIGCHLD,SIG_IGN); /* ignore child */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP,signalHandler); /* catch hangup signal */
    signal(SIGTERM,signalHandler); /* catch kill signal */
}


static void signalHandler(int sig) /* signal handler function */
{
    switch(sig)
    {
    case SIGHUP:
        printf("Received signal SIGHUP\n");
        break;

    case SIGTERM:
        printf("Received signal SIGTERM\n");
        QCoreApplication::quit();
        break;
    }
}