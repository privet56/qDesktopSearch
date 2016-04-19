#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QTextEdit>
#include "cfg.h"

class LogWnd;

class logger : public QObject
{
    Q_OBJECT

public:
    //see also on the bottom
    Q_ENUMS(LogLevel)
    enum LogLevel { DBG, INF, WRN, ERR };

    void SetLogWnd(LogWnd* pTeLog);
    cfg* GetCfg();

protected:
    QString m_sPathName;
    LogWnd* m_pTeLog;
    cfg* m_pCfg;

public:
    static QString level2str(LogLevel level);
    static QString level2color(LogLevel level);

public:
    explicit logger(cfg* pCfg, LogWnd* pTeLog, QObject *parent = 0);

    static QString t_elapsed(int iElapsedMSecs);

signals:

public slots:
    void log(QString s, logger::LogLevel level);
    void dbg(QString s);
    void inf(QString s);
    bool wrn(QString s);
    bool err(QString s);

};

//see also qRegisterMetaType<logger::LogLevel>(); in the cpp
Q_DECLARE_METATYPE( logger::LogLevel )

#endif // LOG_H
