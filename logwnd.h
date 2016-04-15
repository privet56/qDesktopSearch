#ifndef LOGWND_H
#define LOGWND_H

#include <QWidget>
#include <QTextEdit>
#include <QStatusBar>
#include "logger.h"

class LogWnd : public QTextEdit
{
    Q_OBJECT
public:
    explicit LogWnd(QWidget *parent = 0);
    void SetStatusBar(QStatusBar* pStatusBar);
    void AddMsg(QString s, logger::LogLevel level);

protected:
    QStatusBar* m_pStatusBar;

signals:

public slots:
};

#endif // LOGWND_H
