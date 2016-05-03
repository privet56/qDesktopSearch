#include "f.h"
#include "str.h"
#include <QDir>
#include <QChar>
#include <QFileInfo>
#include <QStringList>

f::f(QObject *parent) : QObject(parent)
{

}

bool f::emptydir(QString sDir, logger* pLog)
{
    QDir src(sDir);
    QFileInfoList contents = src.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, contents)
    {
        QString fileName = fileInfo.fileName();
        QString srcFilePath = src.absoluteFilePath(fileName);
        if (fileInfo.isDir())
        {
            QDir dstsubdir(srcFilePath);
            emptydir(srcFilePath, pLog);
            if(!src.rmdir(fileName))
                pLog->wrn("emptydir:!dir.rmdir("+fileName+"):in:'"+sDir+"'");
        }
        else if (fileInfo.isFile())
        {
            if(!QFile::remove(srcFilePath))
                pLog->wrn("emptydir:!file.remove:"+srcFilePath);
        }
        else
        {
            pLog->wrn("emptydir:unexpected:"+srcFilePath);
        }
    }
    return true;
}

QString f::fileSizeAsString(qint64 size)
{
    QString unit;
    double newSize;

    if (size < 1024) {
        newSize = size;
        unit = tr("bytes");
    } else if (size < 1024 * 1024) {
        newSize = (double)size / (double)1024;
        unit = tr("kB");
    } else if (size < 1024 * 1024 * 1024) {
        newSize = (double)size / (double)(1024 * 1024);
        unit = tr("MB");
    } else {
        newSize = (double)size / (double)(1024 * 1024 * 1024);
        unit = tr("GB");
    }

    return QString(QLatin1String("%1 %2")).arg(newSize, 0, 'f', 1).arg(unit);
}

void f::emptydir(QString sDir, QString sPattern, bool bRecursive, int& iDeleteds, int& iDeletionFaileds)
{
    QDir d(sDir);
    d.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList filters;
    filters << sPattern.split(' ');
    d.setNameFilters(filters);
    QStringList fs = d.entryList();

    for(int i=0; i < fs.length(); i++)
    {
        QString sFN(str::makeAbsFN(sDir,fs.at(i)));
        QFileInfo f(sFN);
        if(f.isDir())
        {
            if(bRecursive)
            {
                emptydir(sFN, sPattern, bRecursive, iDeleteds, iDeletionFaileds);
            }
        }
        else if(f.isFile())
        {
            if(!QFile::remove(sFN))
                iDeletionFaileds++;
            else
                iDeleteds++;
        }
        else
        {
            //should not happen on win
        }
    }
}

QString f::getFC(QString sAbsFN, logger* pLog/*=nullptr*/)
{
    QFile f(sAbsFN);
    if(!f.exists())
    {
        if(pLog)pLog->wrn("GetFC fnf:"+sAbsFN);
        return "";
    }
    if(!f.open(QFile::ReadOnly))
    {
        if(pLog)pLog->wrn("GetFC !open:"+sAbsFN);
        return "";
    }
    QByteArray buff = f.readAll();
    f.close();
    QString sFC(buff);
    if(str::isempty(sFC, false))
    {
        if(pLog)pLog->wrn("GetFC isEmpty:"+sAbsFN);
    }
    return sFC;
}
