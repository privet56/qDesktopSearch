#include "lucy.h"
#include "logger.h"
#include "cfg.h"
#include "str.h"
#include "f.h"
#include "analyzer.h"
#include <QApplication>

lucy::lucy(logger* pLogger, QObject *parent) : QObject(parent), m_pLogger(pLogger), m_pAnalyzer(nullptr), m_pDirectory(nullptr)
{

}

void lucy::close(bool bDeleteCompleteIndex/*=false*/)
{
    {
        if( m_pDirectory)
        {
            m_pDirectory->close();
            delete m_pDirectory;
        }
        m_pDirectory = nullptr;
    }
    {
        if (m_pAnalyzer)
        {
            delete m_pAnalyzer;
        }
        m_pAnalyzer = nullptr;
    }
    if(bDeleteCompleteIndex)
    {
        QDir src(this->m_sDirIndex);
        if(!f::emptydir(this->m_sDirIndex, this->m_pLogger) || !src.removeRecursively())
            m_pLogger->wrn("could not delete idx "+this->m_sDirIndex);
        else
            m_pLogger->inf("idx deleted "+this->m_sDirIndex);
    }
}

lucy::~lucy()
{
    close();
}

QString lucy::idxDir4Dir2Index(QString sDir2Index)
{
    sDir2Index = str::normalizePath(sDir2Index, false);
    QString sBaseDirIdx(str::makeAbsFN(qApp->applicationDirPath(), "idx"));
    cfg::mkdirfull(sBaseDirIdx);
    QString sIdxSubDir = QString::number(qHash(sDir2Index));
    QString sAbsDirIdx = str::makeAbsFN(sBaseDirIdx, sIdxSubDir);
    return sAbsDirIdx;
}

void lucy::open(QString sDir2Index)
{
    sDir2Index = str::normalizePath(sDir2Index, false);
    QString sAbsDirIdx(lucy::idxDir4Dir2Index(sDir2Index));
    cfg::mkdirfull(sAbsDirIdx);

    this->m_sDir2Index = sDir2Index;
    this->m_sDirIndex  = sAbsDirIdx;

    {
        QString sAbsFNLock(str::makeAbsFN(this->m_sDirIndex, "write.lock"));
        QFile::remove(sAbsFNLock); //file is there if app crashed
        if(QFile::exists(sAbsFNLock))
            m_pLogger->err("index locked:"+sAbsFNLock);
    }

    m_pAnalyzer     = lucy::getNewAnalyzer();
    m_pDirectory    = FSDirectory::getDirectory(sAbsDirIdx.toLatin1());
}
lucene::analysis::Analyzer* lucy::getAnalyzer()
{
    return this->m_pAnalyzer;
}
Directory* lucy::getDirectory()
{
    return this->m_pDirectory;
}
void lucy::setDirectory(Directory* pDirectory)
{
    this->m_pDirectory = pDirectory;
}
Analyzer* lucy::getNewAnalyzer()
{
    //TODO: handle . as whitespace
    return  new analyzer();//WhitespaceAnalyzer|StopAnalyzer|StandardAnalyzer|SimpleAnalyzer
}
