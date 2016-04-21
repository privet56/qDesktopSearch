#include "f.h"
#include <QDir>
#include <QChar>

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
