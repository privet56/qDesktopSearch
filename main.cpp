#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTextStream>
#include "cfg.h"
#include "logger.h"
#include "jvm.h"
#include "lucy.h"
#include "indexer.h"
#include "tray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //TODO: start as admin?
    //TODO: singleInstance!

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

//tray
    tray trayIcon(QIcon(":/res/bun.png"), &w);
    trayIcon.setToolTip("qDestkopSearch");
    trayIcon.show();

    w.show();
    //splash.finish(&w);    //let MainWindow handle it

    QObject::connect(qApp, SIGNAL(lastWindowClosed()), &_indexer, SLOT(onQuit()));

    return a.exec();
}
