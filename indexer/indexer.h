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
    explicit indexer(logger* pLogger, jvm* pJvm/*, lucyindexer* pLucyIndexer*/, QObject *parent = 0);
    ~indexer();
    QMap<QString, indexerThread*>* getIndexers();
    void add(QString sDir2Index);
    bool isIndexing(QString sDir2Index);
    void removeIndex(QString sDir2Index);
    void stopAll();
    bool isStoppedAll();
    void init();

protected:
    QMap<QString, indexerThread*> m_pWorkers;
    logger* m_pLogger;
    jvm* m_pJvm;

signals:

public slots:
    void onThreadFinished();
    void onQuit();

};

#endif // INDEXER_H
