#include "tray.h"
#include <QMenu>
#include "mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>

static tray* __trayIcon = 0;

tray::tray(QObject *parent) : QSystemTrayIcon(parent), m_paHideorShow(nullptr)
{
    __trayIcon = this;
}

tray::tray(const QIcon &icon, QObject *parent) : QSystemTrayIcon(icon, parent)
{
    __trayIcon = this;
    m_movie.setFileName(":/res/bunny_hopps.gif");
    connect(&m_movie, SIGNAL(updated(const QRect&)),this,SLOT(onIconUpdate()));
    m_movie.start();

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
}

void tray::setup()
{
    this->setToolTip("qDesktopSearch");
    this->show();

    QWidget* parentWnd = (QWidget*)parent();
    QMenu* menu = new QMenu(parentWnd);

    {
        QAction* quitAction = new QAction(QIcon(":/res/exit.png"), "Quit", this);
        connect(quitAction,SIGNAL(triggered()), this->parent()/*the main window*/, SLOT(close()));
        menu->addAction(quitAction);
    }
    {
        QAction* mascotAction = new QAction(QIcon(":/res/bun.png"), "Show Mascot...", this);
        connect(mascotAction,SIGNAL(triggered()), this, SLOT(on_actionShow_Mascot()));
        menu->addAction(mascotAction);
    }
    {
        QAction* aboutAction = new QAction(QIcon(":/res/qbun.png"), "About...", this);
        connect(aboutAction,SIGNAL(triggered()), this, SLOT(onAbout()));
        menu->addAction(aboutAction);
    }
    {
        m_paHideorShow = new QAction(QIcon(":/res/expander_ic_minimized.9.png"), "Minimize to Tray", this);
        m_paHideorShow->setEnabled(true);
        connect(m_paHideorShow,SIGNAL(triggered()),this,SLOT(hide_show()));
        menu->addAction(m_paHideorShow);
    }
    this->setContextMenu(menu);
}

void tray::hide_show()
{
    QWidget* parentWnd = (QWidget*)parent();
    bool isVisible = !parentWnd->isVisible();
    parentWnd->setVisible(isVisible);

    m_paHideorShow->setIcon(isVisible ? QIcon(":/res/expander_ic_minimized.9.png") : QIcon(":/res/expander_ic_maximized.9.png"));
    m_paHideorShow->setText(isVisible ? "Minimize to Tray" : "Restore Main Window");

    if(!isVisible)
    {
        showMessage("Hint", "Your app is still running in background...            ", QSystemTrayIcon::Information, 1000);
    }
}
void tray::onAbout()
{
    showIfHidden();
    QMessageBox::aboutQt((QWidget*)parent(), "qDesktopSearch");
}
void tray::showIfHidden()
{
    QWidget* parentWnd = (QWidget*)parent();

    if(!parentWnd || parentWnd->isVisible())
    {
        return;
    }
    this->hide_show();
}

void tray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        showIfHidden();
    }
}

void tray::onIconUpdate()
{
    QIcon icon(m_movie.currentPixmap());
    this->setIcon(icon);
    QWidget* parentWnd = (QWidget*)parent();
    if(parentWnd)
    {
        QMainWindow* pMainWnd = (QMainWindow*)parentWnd; //TODO: q_cast
        pMainWnd->setWindowIcon(icon);
    }
}

tray* tray::getTrayIcon()
{
    return __trayIcon;
}

void tray::on_actionShow_Mascot()
{
    showIfHidden();

    QWidget* parentWnd = (QWidget*)parent();
    if(parentWnd)
    {
        MainWindow* pMainWnd = (MainWindow*)parentWnd; //TODO: q_cast
        pMainWnd->on_actionShow_Mascot_triggered();
    }
}
