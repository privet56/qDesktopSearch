#ifndef INDEXER_H
#define INDEXER_H

#include <QObject>
#include "indexerthread.h"
#include "jvm.h"
#include "lucyindexer.h"
#include "logger.h"

class indexer : public QObject
{
    Q_OBJECT
public:
    explicit indexer(logger* pLogger, jvm* pJvm, lucyindexer* pLucyIndexer, QObject *parent = 0);
    void add(QString sDir2Index);
    bool isIndexing(QString sDir2Index);

protected:
    QMap<QString, indexerThread*> m_pWorkers;
    logger* m_pLogger;
    lucyindexer* m_pLucyIndexer;
    jvm* m_pJvm;

signals:

public slots:
    void onThreadFinished();

};

#endif // INDEXER_H
