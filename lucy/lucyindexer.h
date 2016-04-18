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

class lucyindexer : public lucy
{
    Q_OBJECT
public:
    lucyindexer(logger* pLogger, QObject *parent = 0);
    ~lucyindexer();
    void open();
    int getNrOfIndexedFiles();
    int getNrOfFilesInIndex();
    bool isIndexed(QString sAbsPathName, QFileInfo finfo);
    void index(QString sAbsPathName, QMap<QString, QStringList>* pMetaContents, QFileInfo finfo);
    void onIndexerThreadFinished();
    QString getIdFNandDATE(QString sAbsPathName, QFileInfo finfo);

protected:
    //IndexWriter* m_pIndexWriter;
    IndexModifier* m_pIndexWriter;
    int m_iIndexedFiles;

};

#endif // LUCYINDEXER_H
