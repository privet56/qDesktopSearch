#include "indexer.h"
#include "globalinclude.h"
#include "str.h"
#include "f.h"
#include "indexerworker.h"
#include "indexerthread.h"

indexer::indexer(logger* pLogger, jvm* pJvm/*, lucyindexer* pLucyIndexer*/, QObject *parent) : QObject(parent),
    m_pLogger(pLogger), m_pJvm(pJvm)//, m_pLucyIndexer(pLucyIndexer)
{

}

void indexer::init()
{
    QString sDirs2Index(this->m_pLogger->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
    for(int i=0;i<sl.length();i++)
    {
        add(sl.at(i));
    }
}

void indexer::add(QString sDir2Index)
{
    if(str::isempty(sDir2Index))
    {
        m_pLogger->inf("not indexing "+sDir2Index);
        return;
    }

    QDir d(sDir2Index);
    if(!d.exists(sDir2Index))
    {
        m_pLogger->inf("not indexing invalid path "+sDir2Index);
        return;
    }

    sDir2Index = str::normalizePath(sDir2Index, false);
    if(this->isIndexing(sDir2Index))return;

    //QThread::msleep(999/*milliseconds*/);

    indexerWorker* pWorker = new indexerWorker(sDir2Index, m_pLogger, m_pJvm/*, m_pLucyIndexer*/);
    //pWorker->openIndex();
    indexerThread* pThread = new indexerThread(pWorker, m_pLogger, m_pJvm);

    //TODO: fix! it blocks!
    connect(this    ,SIGNAL(fillIdxInfo(IdxInfo*)),  pWorker, SLOT(fillIdxInfo(IdxInfo*)), Qt::BlockingQueuedConnection);

    pWorker->moveToThread(pThread);
    connect(pThread, SIGNAL(started()) , pWorker, SLOT(doWork()));
    connect(pWorker, SIGNAL(finished()), this   , SLOT(onThreadFinished()));
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
    QObject* pSignalSender = sender();
    indexerWorker* pWorker = qobject_cast<indexerWorker*>(pSignalSender);
    //TODO: remove from m_pWorkers!
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
        {
            return false;
        }
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

void indexer::removeIndex(QString sDir2Index)
{
    sDir2Index = str::normalizePath(sDir2Index, false);
    if(!this->m_pWorkers.contains(sDir2Index))
    {
        m_pLogger->inf("cannot remove idx for '"+sDir2Index+"'");
        return;
    }
    indexerThread* pThread = this->m_pWorkers[sDir2Index];
    if(this->m_pWorkers.remove(sDir2Index) != 1)
    {
        m_pLogger->inf("cannot remove idx from idxs for '"+sDir2Index+"'");
    }

    pThread->requestInterruption();
    while(pThread->isRunning())
    {
        QThread::msleep(99/*milliseconds*/);
    }
    pThread->close(true/*'true' does not work here*/);
    delete pThread;
    pThread = nullptr;

    {
        QString sAbsDirIdx(lucy::idxDir4Dir2Index(sDir2Index));
        QDir src(sAbsDirIdx);
        if(src.exists())
        {
            if(!f::emptydir(sAbsDirIdx, this->m_pLogger) || !src.removeRecursively())
                m_pLogger->wrn("could not delete idx "+sAbsDirIdx);
            else
                m_pLogger->inf("idx deleted "+sAbsDirIdx);
        }
    }
}

void indexer::getIdxInfo(IdxInfo* pIdxInfo, QString sDir2Index, bool bAskLucy)
{
    pIdxInfo->fill(sDir2Index);

    if(bAskLucy)
    {
        indexerThread* pThread = this->m_pWorkers[sDir2Index];
        if(pThread && pThread->getWorker())
        {
            indexerWorker* pWorker = pThread->getWorker();
            lucyindexer* pIndexer  = pWorker->getIndexer();
            //connect(this    ,SIGNAL(fillIdxInfo(IdxInfo*)),  pWorker, SLOT(fillIdxInfo(IdxInfo*))/*, Qt::BlockingQueuedConnection*/);
            //emit fillIdxInfo(pIdxInfo);
            pIndexer->fillIdxInfo(pIdxInfo);
            //disconnect(this ,SIGNAL(fillIdxInfo(IdxInfo*)),  pWorker, SLOT(fillIdxInfo(IdxInfo*)));
        }
        else
        {
            m_pLogger->wrn("could not find thread for '"+sDir2Index+"'");
        }
    }
}
