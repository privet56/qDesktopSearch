#ifndef INDEXERTHREAD_H
#define INDEXERTHREAD_H

#include <QObject>
#include <QThread>
#include "jvm.h"
#include "logger.h"
#include "indexerworker.h"

class indexerThread : public QThread
{
    Q_OBJECT
public:

    explicit indexerThread(indexerWorker* pWorker, logger* pLogger, jvm* pJvm, QObject *parent = 0);
    void close(bool bDeleteCompleteIndex=false);
    indexerWorker* getWorker();
    ~indexerThread();

protected:
    indexerWorker* m_pWorker;
    logger* m_pLogger;
    jvm* m_pJvm;

signals:

public slots:
};

#endif // INDEXERTHREAD_H
