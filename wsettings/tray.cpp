#include "tray.h"

static tray* __trayIcon = 0;

tray::tray(QObject *parent) : QSystemTrayIcon(parent)
{
    __trayIcon = this;
}

tray::tray(const QIcon &icon, QObject *parent) : QSystemTrayIcon(icon, parent)
{
    __trayIcon = this;
}

tray* tray::getTrayIcon()
{
    return __trayIcon;
}
