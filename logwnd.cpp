#include "logwnd.h"
#include <QScrollBar>

LogWnd::LogWnd(QWidget *parent) : QTextEdit(parent), m_pStatusBar(nullptr)
{

}

void LogWnd::SetStatusBar(QStatusBar* pStatusBar)
{
     this->m_pStatusBar = pStatusBar;
}

void LogWnd::AddMsg(QString s, logger::LogLevel level)
{
    bool bIsSameLine = (s.length() < 2);
    static bool __bLastLineWasIsSameLine = false;   //TODO: should be member

    QString s2log = bIsSameLine ? s : "<font color='"+logger::level2color(level)+"'>"+s+"</font>";
    if(bIsSameLine && __bLastLineWasIsSameLine)
    {
        this->insertHtml(s2log);
    }
    else
    {
        this->append(s2log);
    }

    QScrollBar *sb = this->verticalScrollBar();
    sb->setValue(sb->maximum());

    __bLastLineWasIsSameLine = bIsSameLine;

    if(!bIsSameLine && this->m_pStatusBar)
    {
        this->m_pStatusBar->showMessage(s);
    }

    //TODO: check if too much in the teLog win!
}
