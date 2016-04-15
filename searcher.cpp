#include "searcher.h"

searcher::searcher(QObject *parent) : QObject(parent)
{

}

int searcher::getHitCount()
{
    return 1;
}

QString searcher::GetHitAttr(int iHitNr, QString sAttrName)
{
    return QString("dummy hit "+QString::number(iHitNr)+" attr "+sAttrName);
}

QString searcher::GetHitEnv(int iHitNr)
{
    return QString("dummy <b><font color='red'> "+QString::number(iHitNr)+" hitenv</font></b> dummy...");
}
