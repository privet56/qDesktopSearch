#ifndef BALLOON_H
#define BALLOON_H

#include <QWidget>

class balloon : public QWidget
{
    Q_OBJECT
public:
    explicit balloon(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent*);

signals:

public slots:
};

#endif // BALLOON_H
