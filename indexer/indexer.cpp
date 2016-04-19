#include "indexer.h"
#include "str.h"
#include "indexerworker.h"
#include "indexerthread.h"

indexer::indexer(logger* pLogger, jvm* pJvm/*, lucyindexer* pLucyIndexer*/, QObject *parent) : QObject(parent),
    m_pLogger(pLogger), m_pJvm(pJvm)//, m_pLucyIndexer(pLucyIndexer)
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

    indexerWorker* pWorker = new indexerWorker(sDir2Index, m_pLogger, m_pJvm/*, m_pLucyIndexer*/);
    pWorker->openIndex();
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
    //QObject* pSignalSender = sender();
    //indexerWorker* pWorker = qobject_cast<indexerWorker*>(pSignalSender);
}

void indexer::stopAll()
{
    QMapIterator<QString, indexerThread*> i(m_pWorkers);
    while (i.hasNext())
    {
        i.next();
        i.value()->requestInterruption();
    }
}
bool indexer::isStoppedAll()
{
    QMapIterator<QString, indexerThread*> i(m_pWorkers);
    while (i.hasNext())
    {
        i.next();
        if(i.value()->isRunning())      //if(!i.value()->isFinished()) would be an alternative check
            return false;
    }
    return true;
}

void indexer::onQuit()
{
    this->stopAll();
    while(!this->isStoppedAll())
    {
        ;
    }
    {   //cleanup
        QMapIterator<QString, indexerThread*> i(m_pWorkers);
        while (i.hasNext())
        {
            i.next();
            delete i.value();
        }
        m_pWorkers.clear();
    }
}

QMap<QString, indexerThread*>* indexer::getIndexers()
{
    return &(this->m_pWorkers);
}

indexer::~indexer()
{
    {   //cleanup
        QMapIterator<QString, indexerThread*> i(m_pWorkers);
        while (i.hasNext())
        {
            i.next();
            i.value()->close();
            delete i.value();
        }
        m_pWorkers.clear();
    }
}
