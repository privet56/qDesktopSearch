#include "lucy.h"
#include "logger.h"
#include "cfg.h"
#include "str.h"
#include <QApplication>

lucy::lucy(logger* pLogger, QObject *parent) : QObject(parent), m_pLogger(pLogger), m_pAnalyzer(nullptr), m_pDirectory(nullptr)
{

}

lucy::~lucy()
{
    if( m_pDirectory)
        m_pDirectory->close();
}

void lucy::open()
{
    QString sDirIdx(str::makeAbsFN(qApp->applicationDirPath(), "idx"));
    cfg::mkdirfull(sDirIdx);
    m_pAnalyzer     = new WhitespaceAnalyzer();//StandardAnalyzer();   //alternative: SimpleAnalyzer
    m_pDirectory    = FSDirectory::getDirectory(sDirIdx.toLatin1());
}
