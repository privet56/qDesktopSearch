QT       += core gui
#does not work on win
#QT       += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qDesktopSearch
TEMPLATE = app

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DEFINES += DEBUG
}

#would be cool if my libs would also be compiled with this flag
#DEFINES += _ATL_XP_TARGETING

#avoid zlib warnings
DEFINES += _CRT_SECURE_NO_WARNINGS
#make clucene faster
DEFINES += _REENTRANT
DEFINES += NOMINMAX
DEFINES += WIN32_LEAN_AND_MEAN
DEFINES += WIN64_LEAN_AND_MEAN
DEFINES += MAKE_CLUCENE_SHARED_LIB

#//TODO: activate & config for porterStemmer
#DEFINES += _MBCS
#DEFINES += _UNICODE

include(qDesktopSearch_3rdparty.pri)

SOURCES += main.cpp \
    mainwindow.cpp \
    wsearch.cpp \
    wsettings.cpp \
    cfg.cpp \
    emitter.cpp \
    logger.cpp \
    logwnd.cpp \
    util/str.cpp \
    wsearch/wsearchresultmodel.cpp \
    indexer/indexer.cpp \
    indexer/indexerthread.cpp \
    indexer/indexerworker.cpp \
    indexer/jvm.cpp \
    lucy/lucy.cpp \
    lucy/lucyindexer.cpp \
    lucy/lucysearcher.cpp \
    searcher/searcher.cpp \
    searcher/seacherHighlightFormatter.cpp \
    searcher/wsearchitemdelegate.cpp \
    wsettings/wsettingdirs2indexmodel.cpp \
    util/f.cpp \
    wsearch/wfieldsmodel.cpp \
    wsettings/tray.cpp \
    util/balloon.cpp

HEADERS  += mainwindow.h \
    wsearch.h \
    wsettings.h \
    res.rc \
    resource.h \
    cfg.h \
    emitter.h \
    logger.h \
    logwnd.h \
    util/str.h \
    wsearch/wsearchresultmodel.h \
    indexer/indexer.h \
    indexer/indexerthread.h \
    indexer/indexerworker.h \
    indexer/jvm.h \
    lucy/lucy.h \
    lucy/lucyindexer.h \
    lucy/lucysearcher.h \
    searcher/searcher.h \
    searcher/seacherHighlightFormatter.h \
    globalinclude.h \
    searcher/wsearchitemdelegate.h \
    wsettings/wsettingdirs2indexmodel.h \
    util/f.h \
    wsearch/wfieldsmodel.h \
    wsettings/tray.h \
    util/balloon.h

FORMS    += mainwindow.ui \
    wsearch.ui \
    wsettings.ui

RESOURCES += res.qrc

DISTFILES += ico.ico

RC_FILE = res.rc

INCLUDEPATH += ./lucy
INCLUDEPATH += ./indexer
INCLUDEPATH += ./searcher
INCLUDEPATH += ./wsearch
INCLUDEPATH += ./wsettings
INCLUDEPATH += ./util
