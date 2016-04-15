#include "wsettings.h"
#include "ui_wsettings.h"

wSettings::wSettings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wSettings)
{
    ui->setupUi(this);
}

wSettings::~wSettings()
{
    delete ui;
}
