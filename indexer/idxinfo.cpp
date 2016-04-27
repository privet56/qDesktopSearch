#include "idxinfo.h"
#include "indexer.h"
#include "lucyindexer.h"

IdxInfo::IdxInfo(QObject *parent) : QObject(parent)
{
    this->m_indexDirSize = -1;
    this->m_indexDirNrOfFiles = -1;
    this->m_indexNrOfFilesInIndex = -1;
    this->m_indexNrOfIndexedFilesInCurrentLoop = -1;
}

//GET
QString IdxInfo::dir2Index()
{
    return this->m_dir2Index;
}
QString IdxInfo::indexDir()
{
    return this->m_indexDir;
}
size_t IdxInfo::indexDirSize()
{
    return this->m_indexDirSize;
}
size_t IdxInfo::indexDirNrOfFiles()
{
    return this->m_indexDirNrOfFiles;
}
size_t IdxInfo::indexNrOfFilesInIndex()
{
    return this->m_indexNrOfFilesInIndex;
}
bool IdxInfo::indexIsNew()
{
    return this->m_indexIsNew;
}
size_t IdxInfo::indexNrOfIndexedFilesInCurrentLoop()
{
    return this->m_indexNrOfIndexedFilesInCurrentLoop;
}

//SET
void IdxInfo::setDir2Index(QString s)
{
    this->m_dir2Index = s;
}
void IdxInfo::setIndexDir(QString s)
{
    this->m_indexDir = s;
}
void IdxInfo::setIndexDirSize(size_t i)
{
    this->m_indexDirSize = i;
}
void IdxInfo::setIndexDirNrOfFiles(size_t i)
{
    this->m_indexDirNrOfFiles = i;
}
void IdxInfo::setIndexNrOfFilesInIndex(size_t i)
{
    this->m_indexNrOfFilesInIndex = i;
}
void IdxInfo::setIndexIsNew(bool b)
{
    this->m_indexIsNew = b;
}
void IdxInfo::setIndexNrOfIndexedFilesInCurrentLoop(size_t i)
{
    this->m_indexNrOfIndexedFilesInCurrentLoop = i;
}

//the real meat

void IdxInfo::fill(QString dir2Index)
{
    this->m_dir2Index   = str::normalizePath(dir2Index, false);
    this->m_indexDir    = lucy::idxDir4Dir2Index(dir2Index);

    fillDirInfo(this->m_indexDir);

    /*if(pIndexer)
    {
        //ATTENZIONE: 'BlockingQueuedConnection' is important here, but works only if target is in another thread!
        connect(this,SIGNAL(fillIdxInfo(IdxInfo*)),pIndexer,SLOT(fillIdxInfo(IdxInfo*)), Qt::BlockingQueuedConnection);
        emit fillIdxInfo(this);
        disconnect(this,SIGNAL(fillIdxInfo(IdxInfo*)),pIndexer,SLOT(fillIdxInfo(IdxInfo*)));
    }*/
}

void IdxInfo::fillDirInfo(QString dir2Index)
{
    this->m_indexDirSize        = 0;
    this->m_indexDirNrOfFiles   = 0;

    QString sSuffix = (dir2Index.endsWith(QDir::separator()) || dir2Index.endsWith('/')) ? QString("") : QDir::separator();
    QDirIterator it(dir2Index+sSuffix, QStringList() << "*", QDir::Files | QDir::Dirs | QDir::NoDotDot | QDir::NoDot, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString sEntry(it.next());
        sEntry = str::normalizePath(sEntry, false);
        QFileInfo finfo(sEntry);

        this->m_indexDirNrOfFiles++;        //counts actually also subdirs...
        this->m_indexDirSize += finfo.size();
    }
}
