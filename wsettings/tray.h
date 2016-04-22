#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QSystemTrayIcon>

class QBalloonTip;

class tray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit tray(QObject *parent = 0);
    explicit tray(const QIcon &icon, QObject *parent = Q_NULLPTR);

    static tray* getTrayIcon();

signals:

public slots:
};

#endif // TRAY_H
