#include "str.h"
#include <QDir>
#include <QChar>

str::str(QObject *parent) : QObject(parent)
{

}

QString str::mult(QString s, int iMultiplicator)
{
    return s.repeated(iMultiplicator);
}

QString str::normalizePath(QString sPath, bool bLowerCase)
{
    sPath = sPath.replace('\\','/').trimmed();
    if(bLowerCase)
        sPath = sPath.toLower();
    return sPath;
}

QString str::makeAbsFN(QString sBase, QString sAppend)
{
    QChar sep = QDir::separator();
    if(sBase.startsWith("http:") || sBase.startsWith("https:") || sBase.startsWith("ftp:"))
    {
        sep = '/';
    }

    sBase   = sBase.replace('\\'  , sep).trimmed();
    sBase   = sBase.replace('/'   , sep);
    sAppend = sAppend.replace('\\', sep).trimmed();
    sAppend = sAppend.replace('/' , sep);

    if(sAppend.isNull() || sAppend.isEmpty())   return sBase;
    if(sBase.isNull() || sBase.isEmpty())       return sAppend;

    if((sAppend.length() > 1) && ((sAppend.at(1) == ':') || sAppend.startsWith(QString(QDir::separator())+QDir::separator())))
    {
        return sAppend;
    }
    bool bBase = sBase.endsWith(sep);
    bool bAppd = sAppend.startsWith(sep);
    if(bBase && bAppd)
    {
        return sBase.left(sBase.length()-1) + sAppend;
    }
    if(bBase || bAppd)
    {
        return sBase + sAppend;
    }
    return sBase + sep + sAppend;
}

bool str::isempty(QString s, bool bTrim)
{
    if(s.isNull() || s.isEmpty())return true;
    if(!bTrim)return false;
    s = s.trimmed();
    if(s.isNull() || s.isEmpty())return true;
    return false;
}
