#include "indexerthread.h"

indexerThread::indexerThread(indexerWorker* pWorker, logger* pLogger, jvm* pJvm, QObject *parent) : QThread(parent),
    m_pLogger(pLogger), m_pJvm(pJvm), m_pWorker(pWorker)
{

}

void indexerThread::close()
{
    if(m_pWorker)
    {
        m_pWorker->close();
        delete m_pWorker;
    }
    m_pWorker = nullptr;
}

indexerThread::~indexerThread()
{
    close();
}
indexerWorker* indexerThread::getWorker()
{
    return this->m_pWorker;
}
