#ifndef LUCYINDEXER_H
#define LUCYINDEXER_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QDirIterator>
#include <QMap>
#include <QStringList>
#include <QFileInfo>
#include "lucy.h"
#include "idxinfo.h"
#include "qindexmodifier.h"

class lucyindexer : public lucy
{
    Q_OBJECT
public:
    lucyindexer(logger* pLogger, QObject *parent = 0);
    ~lucyindexer();
    void open(QString sDir2Index);
    virtual void close(bool bDeleteCompleteIndex=false);
    int getNrOfIndexedFiles();
    int getNrOfFilesInIndex();
    bool isIndexed(QString sAbsPathName, QFileInfo finfo);
    void index(QString sAbsPathName, QMap<QString, QStringList>* pMetaContents, QFileInfo finfo);
    void onIndexerThreadFinished(bool bIndexerLoopFinished=false);
    QString getIdFNandDATE(QString sAbsPathName, QFileInfo finfo);
    QIndexModifier* getIndexer();

    QString metaName(QString sRawMetaName);
    QString getIndexDir();
    int32_t delDoc(QString sFieldName, QString sFieldContent);

protected:
    QIndexModifier* m_pIndexWriter;      //the alternative would be IndexWriter*
    int m_iIndexedFiles;
    bool m_bNewIndex;

public slots:
    void fillIdxInfo(IdxInfo* idxi);

signals:

};

#endif // LUCYINDEXER_H
