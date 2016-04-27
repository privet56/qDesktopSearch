#include "lucyindexer.h"

lucyindexer::lucyindexer(logger* pLogger, QObject *parent) : lucy(pLogger, parent), m_pIndexWriter(nullptr), m_iIndexedFiles(0), m_bNewIndex(false)
{

}

void lucyindexer::open(QString sDir2Index)
{
    lucy::open(sDir2Index);
    bool bReCreate = !m_pDirectory->fileExists("segments.gen");
    m_bNewIndex = bReCreate;

    //m_pIndexWriter  = new IndexWriter(m_pDirectory, m_pAnalyzer, bReCreate/*recreate*/, true/*closeDirOnShutdown*/);   //is thread safe!

    //http://clucene.sourceforge.net/doc/html/classlucene_1_1index_1_1IndexModifier.html#_details
    //  Note that you cannot create more than one IndexModifier object on the same directory at the same time
    m_pIndexWriter  = new IndexModifier(m_pDirectory, m_pAnalyzer, bReCreate/*recreate*/);
    m_pIndexWriter->setMaxFieldLength(INT_MAX);

    if(!m_bNewIndex)
    {
        //onIndexerThreadFinished();
    }
}

void lucyindexer::close(bool bDeleteCompleteIndex/*=false*/)
{
    if( m_pIndexWriter)
    {
        m_pIndexWriter->close();
        delete m_pIndexWriter;
    }

    m_pIndexWriter = nullptr;
    lucy::close();
}

lucyindexer::~lucyindexer()
{
    close();
}

bool lucyindexer::isIndexed(QString sAbsPathName, QFileInfo finfo)
{
    sAbsPathName = str::normalizePath(sAbsPathName, false);
    QString id_fnAndDate = getIdFNandDATE(sAbsPathName, finfo);
    Term* term = _CLNEW Term(ID_FNANDDATE, id_fnAndDate.toStdWString().c_str());
    TermDocs* en = m_pIndexWriter->termDocs(term);
    bool bDocIsAlreadyIndexed = (en != nullptr) && en->next();
    if(en){_CLDELETE(en);}
    _CLDECDELETE(term);
    return bDocIsAlreadyIndexed;
}

void lucyindexer::index(QString sAbsPathName, QMap<QString, QStringList>* pMetaContents, QFileInfo finfo)
{
    sAbsPathName = str::normalizePath(sAbsPathName, false);
    QString id_fn           = QString::number(qHash(sAbsPathName));
    QString id_fnAndDate    = getIdFNandDATE(sAbsPathName, finfo);

    {   //delete old version, if there
        //TODO: don't delete in NEW index
        //TODO: is qHash enough to identify a record?
        Term* term = _CLNEW Term(ID_FN, id_fn.toStdWString().c_str());
        int32_t iDeletedFiles = m_pIndexWriter->deleteDocuments(term);
        _CLDECDELETE(term);
        if(iDeletedFiles > 1)
        {
            this->m_pLogger->wrn("too much files deleted(iDeletedFiles:"+QString::number(iDeletedFiles)+", id_fn:{"+id_fn+"}) fn:{"+sAbsPathName+"}");
        }
        else if(m_bNewIndex && (iDeletedFiles > 0))
        {
            this->m_pLogger->wrn("too much files in NEW idx deleted(iDeletedFiles:"+QString::number(iDeletedFiles)+", id_fn:{"+id_fn+"}) fn:{"+sAbsPathName+"}");
        }
    }

    Document doc;

    {   //index ids
        doc.add(*_CLNEW Field(ID_FN         , id_fn.toStdWString().c_str()       , Field::STORE_YES | Field::INDEX_UNTOKENIZED));
        doc.add(*_CLNEW Field(ID_FNANDDATE  , id_fnAndDate.toStdWString().c_str(), Field::STORE_YES | Field::INDEX_UNTOKENIZED));
    }

    int config = Field::STORE_YES | Field::INDEX_TOKENIZED;

    QMapIterator<QString, QStringList> i(*pMetaContents);
    while (i.hasNext())
    {
        i.next();
        QString sMetaName(i.key());
        if(sMetaName == "x-parsed-by")
            continue;
        sMetaName = metaName(sMetaName);

        QStringList sMetaValues = i.value();
        for(int iMetaValue = 0; iMetaValue < sMetaValues.size(); ++iMetaValue)
        {
            //TCHAR *fieldName = QStringToTChar(name);  //TODO: check if this is necessary
            //delete [] fieldName;
            doc.add(*_CLNEW Field(sMetaName.toStdWString().c_str(), sMetaValues[iMetaValue].toStdWString().c_str(), config));
        }
    }

    m_pIndexWriter->addDocument(&doc);
    m_iIndexedFiles++;

    if((m_iIndexedFiles % OPTIMIZE_AFTER_INDEXED_FILES) == 0) //TODO: if(win64)->100000 if(win32)->10000
    {
        onIndexerThreadFinished();
    }
}

void lucyindexer::onIndexerThreadFinished(bool bIndexerLoopFinished/*=false*/)
{
    if(m_pIndexWriter &&
       (
        ( m_iIndexedFiles > OPTIMIZE_AFTER_INDEXED_FILES) ||
        (/*(m_iIndexedFiles > OPTIMIZE_AFTER_INDEXED_FILES/10) && */bIndexerLoopFinished)
       )
      )
    {
        //TODO: how to make it better (currently the UI is blocked while optimizing)? ipc?
        QMutexLocker ml(lucy::getIndexerLock());
        m_pLogger->wrn("opt START (new:"+QString::number(m_bNewIndex)+", IdxLoopFinish:"+QString::number(bIndexerLoopFinished)+") "+this->m_sDir2Index);
        QTime t;
        t.start();
        m_pIndexWriter->flush();
        m_pIndexWriter->optimize();
        if(t.elapsed() > 999)
        {
            this->m_pLogger->inf("optimized in "+logger::t_elapsed(t.elapsed())+" "+this->m_sDir2Index);
        }
        else
        {
            m_pLogger->wrn("opt END.. (new:"+QString::number(m_bNewIndex)+", IdxLoopFinish:"+QString::number(bIndexerLoopFinished)+") "+this->m_sDir2Index);
        }
    }
    if(bIndexerLoopFinished)
    {
        this->m_iIndexedFiles = 0;
        this->m_bNewIndex = false;
    }
}
QString lucyindexer::metaName(QString sRawMetaName)
{
    sRawMetaName = sRawMetaName.toLower().trimmed();
    sRawMetaName = sRawMetaName.replace("dc:","");
    sRawMetaName = sRawMetaName.replace(QRegExp("[^a-z0-9\\-]"), "_");
    return sRawMetaName;
}

int lucyindexer::getNrOfIndexedFiles()
{
    return this->m_iIndexedFiles;
}
int lucyindexer::getNrOfFilesInIndex()
{
    if(this->m_pIndexWriter)
    {
        QMutexLocker ml(lucy::getIndexerLock());
        return this->m_pIndexWriter->docCount();
    }
    return 0;
}

QString lucyindexer::getIdFNandDATE(QString sAbsPathName, QFileInfo finfo)
{
    sAbsPathName = str::normalizePath(sAbsPathName, false);
    return QString::number(qHash(sAbsPathName)) + "_" + QString::number(finfo.lastModified().toTime_t());
}

IndexModifier* lucyindexer::getIndexer()
{
    return this->m_pIndexWriter;
}

void lucyindexer::fillIdxInfo(IdxInfo* idxi)
{
    idxi->setIndexNrOfFilesInIndex(this->getNrOfFilesInIndex());
    idxi->setIndexIsNew(this->m_bNewIndex);
    idxi->setIndexNrOfIndexedFilesInCurrentLoop(getNrOfIndexedFiles());
}
