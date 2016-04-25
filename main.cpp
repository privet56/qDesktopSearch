#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <qtsingleapplication.h>
#include "globalinclude.h"
#include "cfg.h"
#include "logger.h"
#include "jvm.h"
#include "lucy.h"
#include "indexer.h"
#include "tray.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if (a.sendMessage(SINGLEINSTANCE_QDESKTOPSEARCH_WAKEUP))
    {
        return 0;   //another instance notified
    }

    //TODO: start as admin?

//splash
    QPixmap pixmap(":/res/bun.png");
    QSplashScreen splash(pixmap, Qt::SplashScreen | Qt::WindowStaysOnTopHint);
#ifndef _DEBUG
    splash.show();
#endif
    splash.showMessage("Loading...", Qt::AlignBottom, Qt::red);

    cfg _cfg(&a);
    logger _log(&_cfg, nullptr, &a);

//css
    {
        QFile data(":/res/css.css");
        QString style;
        if (data.open(QFile::ReadOnly))
        {
            QTextStream styleIn(&data);
            style = styleIn.readAll();
            data.close();
        }
        a.setStyleSheet(style);
    }

//jvm
    jvm _jvm(&_log);
    _jvm.load();

//indexer
    indexer _indexer(&_log, &_jvm);

//wnd
    MainWindow w(&splash, &_log, &_indexer);

    a.setActivationWindow(&w, true);
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &a, SLOT(activateWindow()));
    QObject::connect(qApp, SIGNAL(lastWindowClosed()), &_indexer, SLOT(onQuit()));

//tray
    tray trayIcon(QIcon(":/res/bun.png"), &w);      //TODO: why it is not showing?
    trayIcon.setup();
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &trayIcon, SLOT(showIfHidden()));

    w.show();

    return a.exec();
}
