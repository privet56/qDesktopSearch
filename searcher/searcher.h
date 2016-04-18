#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>

class searcher : public QObject
{
    Q_OBJECT
public:
    explicit searcher(QObject *parent = 0);

    int getHitCount();
    QString GetHitAttr(int iHitNr, QString sAttrName);
    QString GetHitEnv(int iHitNr);

signals:

public slots:
};

#endif // SEARCHER_H
