#ifndef WSETTINGS_H
#define WSETTINGS_H

#include <QFrame>

namespace Ui {
class wSettings;
}

class wSettings : public QFrame
{
    Q_OBJECT

public:
    explicit wSettings(QWidget *parent = 0);
    ~wSettings();

private:
    Ui::wSettings *ui;
};

#endif // WSETTINGS_H
