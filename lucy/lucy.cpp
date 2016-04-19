#include "lucy.h"
#include "logger.h"
#include "cfg.h"
#include "str.h"
#include <QApplication>

lucy::lucy(logger* pLogger, QObject *parent) : QObject(parent), m_pLogger(pLogger), m_pAnalyzer(nullptr), m_pDirectory(nullptr)
{

}

void lucy::close()
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
}

lucy::~lucy()
{
    close();
}

void lucy::open(QString sDir2Index)
{
    sDir2Index = str::normalizePath(sDir2Index, false);
    QString sBaseDirIdx(str::makeAbsFN(qApp->applicationDirPath(), "idx"));
    cfg::mkdirfull(sBaseDirIdx);
    QString sIdxSubDir = QString::number(qHash(sDir2Index));
    QString sAbsDirIdx = str::makeAbsFN(sBaseDirIdx, sIdxSubDir);
    cfg::mkdirfull(sAbsDirIdx);

    this->m_sDir2Index = sDir2Index;
    this->m_sDirIndex  = sAbsDirIdx;

    QFile::remove(str::makeAbsFN(this->m_sDirIndex, "write.lock")); //file is there if app crashed

    m_pAnalyzer     = new WhitespaceAnalyzer();//StandardAnalyzer();   //alternative: SimpleAnalyzer
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
