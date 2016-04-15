#include "indexerthread.h"

indexerThread::indexerThread(indexerWorker* pWorker, logger* pLogger, jvm* pJvm, QObject *parent) : QThread(parent), m_pLogger(pLogger), m_pJvm(pJvm), m_pWorker(pWorker)
{

}
