#include "indexer.h"
#include "str.h"
#include "indexerworker.h"
#include "indexerthread.h"

indexer::indexer(logger* pLogger, jvm* pJvm, lucyindexer* pLucyIndexer, QObject *parent) : QObject(parent), m_pLogger(pLogger), m_pJvm(pJvm), m_pLucyIndexer(pLucyIndexer)
{

}

void indexer::add(QString sDir2Index)
{
    if(str::isempty(sDir2Index))
    {
        m_pLogger->inf("not indexing "+sDir2Index);
        return;
    }

    sDir2Index = str::normalizePath(sDir2Index, false);
    if(this->isIndexing(sDir2Index))return;

    indexerWorker* pWorker = new indexerWorker(sDir2Index, m_pLogger, m_pJvm, m_pLucyIndexer);
    indexerThread* pThread = new indexerThread(pWorker, m_pLogger, m_pJvm);

    pWorker->moveToThread(pThread);
    connect(pThread, SIGNAL(started()), pWorker, SLOT(doWork()));
    connect(pWorker, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    connect(pWorker, SIGNAL(finished()), pThread, SLOT(terminate()));
    connect(pWorker, SIGNAL(finished()), pThread, SLOT(deleteLater()));
    pThread->start();

    this->m_pWorkers[sDir2Index] = pThread;
    m_pLogger->inf("start indexing "+sDir2Index);
}

bool indexer::isIndexing(QString sDir2Index)
{
    return this->m_pWorkers.contains(sDir2Index);
}

void indexer::onThreadFinished()
{
    this->m_pLucyIndexer->onIndexerThreadFinished();

    QObject* pSignalSender = sender();
    indexerWorker* pWorker = qobject_cast<indexerWorker*>(pSignalSender);
    m_pLogger->inf("an indexer thread finished. found#:"+QString::number(pWorker->getNrOfFoundFiles())+" indexed#:"+QString::number(pWorker->getNrOfIndexedFiles())+" filesInIndex:"+QString::number(pWorker->getNrOfFilesInIndex())+" time:"+logger::t_elapsed(pWorker->getIndexTime())+"  dir:"+pWorker->getIndexedDir());
}
