#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QQuickView>
// #include <QApplication>
#include <QGuiApplication>
#include <QQmlContext>

#ifdef MER_EDITION_SAILFISH
#include <sailfishapp.h>
#else
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#endif

#include <QTranslator>

#include "blockmodel.h"

#ifndef TRANSLATION_FOLDER 
#define TRANSLATION_FOLDER ""
#endif

int main(int argc, char *argv[])
{

    QGuiApplication *app;
#ifdef MER_EDITION_SAILFISH
    app = SailfishApp::application(argc, argv);
#else
    // app = new QApplication(argc, argv);
    app = new QGuiApplication(argc, argv);

    {
        QString tr_path(TRANSLATION_FOLDER);

        if ( !tr_path.isEmpty() ) {
            QString locale = QLocale::system().name();
            QTranslator *translator = new QTranslator();

            if ( !translator->load(QLocale(), "exphone", "_", tr_path) ) {
                qWarning() << "Failed to load translation for " << locale << " " << tr_path;
            }

            app->installTranslator(translator);
        }

    }

#endif

    QCoreApplication::setOrganizationName("exphone");
    QCoreApplication::setOrganizationDomain("com.github.jmlich");
    QCoreApplication::setApplicationName("exphone");
    QGuiApplication::setApplicationDisplayName("exPhone");


    BlockModel blockmodel;
    blockmodel.initDb();
    blockmodel.loadAll();

#ifdef MER_EDITION_SAILFISH
    QQuickView *view = SailfishApp::createView();
#else
    QQmlApplicationEngine *view = new QQmlApplicationEngine();
#endif

    view->rootContext()->setContextProperty("BlockModel", &blockmodel);

//#ifdef MER_EDITION_SAILFISH
//    view->setSource(SailfishApp::pathTo("qml/exphone.qml"));
//    view->show();
//#elif UUITK_EDITION
//    view->load("./share/exphone/qml/exphone.qml");
//#else
    view->load(QUrl("qrc:/qml/exphone.qml"));

    if (QQuickWindow* window = qobject_cast<QQuickWindow*>(view->rootObjects().at(0))) {
        window->setIcon(QIcon(":/icons/256x256/exphone.png"));
    }

//#endif

    return app->exec();

}
