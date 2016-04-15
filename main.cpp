#include "mainwindow.h"
#include <QSplashScreen>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "cfg.h"
#include "logger.h"
#include "jvm.h"
#include "lucy.h"
#include "indexer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//splash
    QPixmap pixmap(":/res/bun.png");
    QSplashScreen splash(pixmap, Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    splash.show();
    splash.showMessage("Loading...", Qt::AlignBottom, Qt::red);

    cfg _cfg(&a);
    logger _log(&_cfg, nullptr, &a);

//css
    {
        QFile data(":/css.css");
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

//lucy
    lucyindexer _lucyindexer(&_log);
    _lucyindexer.open();

//indexer
    indexer _indexer(&_log, &_jvm, &_lucyindexer);

//wnd
    MainWindow w(&splash, &_log, &_indexer);
    w.show();
    //splash.finish(&w);    //let MainWindow handle it

    return a.exec();
}
