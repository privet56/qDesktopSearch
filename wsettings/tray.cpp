#include "tray.h"
#include <QMenu>

static tray* __trayIcon = 0;

tray::tray(QObject *parent) : QSystemTrayIcon(parent), m_paHideorShow(nullptr)
{
    __trayIcon = this;
}

tray::tray(const QIcon &icon, QObject *parent) : QSystemTrayIcon(icon, parent)
{
    __trayIcon = this;
}

void tray::setup()
{
    this->setToolTip("qDesktopSearch");
    this->show();

    QWidget* parentWnd = (QWidget*)parent();
    QMenu* menu = new QMenu(parentWnd);

    QAction* quitAction = new QAction(QIcon(":/res/exit.png"), "Quit", this);
    connect(quitAction,SIGNAL(triggered()), this->parent()/*the main window*/, SLOT(close()));
    menu->addAction(quitAction);
    this->setContextMenu(menu);

    m_paHideorShow = new QAction(QIcon(":/res/expander_ic_minimized.9.png"), "Minimize to Tray", this);
    m_paHideorShow->setEnabled(true);
    connect(m_paHideorShow,SIGNAL(triggered()),this,SLOT(hide_show()));
    menu->addAction(m_paHideorShow);

    this->setContextMenu(menu);
}

void tray::hide_show()
{
    QWidget* parentWnd = (QWidget*)parent();
    bool isVisible = !parentWnd->isVisible();
    parentWnd->setVisible(isVisible);

    m_paHideorShow->setIcon(isVisible ? QIcon(":/res/expander_ic_minimized.9.png") : QIcon(":/res/expander_ic_maximized.9.png"));
    m_paHideorShow->setText(isVisible ? "Minimize to Tray" : "Restore Main Window");
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

tray* tray::getTrayIcon()
{
    return __trayIcon;
}
