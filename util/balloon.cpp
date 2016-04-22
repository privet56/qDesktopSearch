#include "balloon.h"
#include <QPen>
#include <QBrush>
#include <QPainter>

balloon::balloon(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::Window |Qt::FramelessWindowHint);
    this->resize(200,100);
}
void balloon::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPen pen( QColor("orange") );
    pen.setWidth(5);
    painter.setPen(pen);
    QBrush brush(QColor("lightblue"));
    painter.setBrush(brush);

    painter.drawRect(0,0,this->width(), this->height()-50);

    pen.setColor(QColor("black"));
    painter.setPen(pen);
    painter.drawText(20,30, "This is a tooltip!");
}
