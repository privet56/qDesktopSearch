#include "logger.h"
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <QThread>
#include <QScrollBar>
#include "emitter.h"
#include "logwnd.h"

logger::logger(cfg* pCfg, LogWnd* pTeLog, QObject *parent) : QObject(parent), m_pCfg(pCfg)
{
    qRegisterMetaType<logger::LogLevel>();

    this->m_pTeLog = pTeLog;

    bool bLog = pCfg->getValue("debug") == "1";

    m_sPathName = bLog ? (QApplication::applicationFilePath() + ".log") : QString::null;
}

//TODO: is qInstallMesssageHandler better?

void logger::log(QString s, logger::LogLevel level)
{
    bool isGuiThread = QThread::currentThread() == QCoreApplication::instance()->thread();
    if (!isGuiThread)
    {
        qDebug() << this->level2str(level) << " " << s;
        static emitter e(this, 0);  //TODO: make member instead of static
        e.emitlog(s, level, this);
        return;
    }

    if(sender() == nullptr) //not yet qDebug't in the above if -> qDebug now!
    {
        qDebug() << this->level2str(level) << " " << s;
    }

    if(s.isNull() || s.isEmpty())
        return;
    bool bIsSameLine = (s.length() < 2);

    if (this->m_pTeLog)
    {
        this->m_pTeLog->AddMsg(s, level);
    }

    static bool __bLastLineWasIsSameLine = false;

    if(!m_sPathName.isNull())
    {
        QFile f(m_sPathName);
        if(f.open( QIODevice::WriteOnly | QFile::Append | QFile::Text))
        {
            QTextStream stream( &f );
            if(bIsSameLine && __bLastLineWasIsSameLine)
            {
                stream << s;
            }
            else
            {
                if(bIsSameLine)
                    stream << this->level2str(level) << " " << s;
                else
                {
                    if(__bLastLineWasIsSameLine)
                        stream << "\n";
                    stream << this->level2str(level) << " " << s << "\n";
                }
            }

            f.close();
        }
        else
        {
            qDebug() << "doLog: !open " << this->m_sPathName;
        }
    }

    __bLastLineWasIsSameLine = bIsSameLine;
}

void logger::dbg(QString s)
{
    this->log(s, LogLevel::DBG);
}
void logger::inf(QString s)
{
    this->log(s, LogLevel::INF);
}
bool logger::wrn(QString s)
{
    this->log(s, LogLevel::WRN);
    return false;
}
bool logger::err(QString s)
{
    this->log(s, LogLevel::ERR);
    return false;
}

QString logger::level2color(LogLevel level)
{
    if(level == LogLevel::DBG)
        return "blue";
    if(level == LogLevel::INF)
        return "green";
    if(level == LogLevel::WRN)
        return "orange";
    if(level == LogLevel::ERR)
        return "red";
    qDebug() << "level2color:unknown level";
    return "red";
}
QString logger::level2str(LogLevel level)
{
    if(level == LogLevel::DBG)
        return "DBG";
    if(level == LogLevel::INF)
        return "INF";
    if(level == LogLevel::WRN)
        return "WRN";
    if(level == LogLevel::ERR)
        return "ERR";
    qDebug() << "level2str:unknown level";
    return "ERR";
}
QString logger::t_elapsed(int iElapsedMSecs)
{
    int iHours=iElapsedMSecs / (1000*60*60);    iElapsedMSecs%=1000*60*60;
    int iMins =iElapsedMSecs / (1000*60);       iElapsedMSecs%=1000*60;
    int iSecs =iElapsedMSecs / (1000);          iElapsedMSecs%=1000;

    QString s;
    bool d=false;
    if(iHours)
        s+=QString::number(iHours) + " hours ", d=true;
    if(iMins || d)
        s+=QString::number(iMins) +" min ", d=true;
    if(iSecs || d)
        s+=QString::number(iSecs) +" secs ";
    s+=QString::number(iElapsedMSecs)+" msecs";
    return s;
}

void logger::SetLogWnd(LogWnd* pTeLog)
{
    this->m_pTeLog = pTeLog;
}

cfg* logger::GetCfg()
{
    return this->m_pCfg;
}
