#include "searcher.h"
#include "lucysearcher.h"
#include "seacherHighlightFormatter.h"

searcher::searcher(QObject *parent) : QObject(parent),
    m_pIndexer(nullptr),
    m_pLog(nullptr),
    m_pMultiSearcher(nullptr),
    m_aquerys(nullptr),
    m_searchables(nullptr),
    m_query(nullptr),
    m_sort(nullptr),
    m_tophits(nullptr),
    m_hits(nullptr)
{
    cleanup(true);
}

int searcher::search(QList<QPair<QString, QString>> lpSearchinputs)
{
    cleanup(false);

    if(lpSearchinputs.size() < 1)
    {
        this->m_pLog->wrn("nothing to search for!");
        return 0;
    }

    QMapIterator<QString, indexerThread*> i(*this->m_pIndexer->getIndexers());
    m_searchables = _CL_NEWARRAY(Searchable*,this->m_pIndexer->getIndexers()->size()+1);
    int iIdx = 0;
    while (i.hasNext())
    {
        i.next();

        lucysearcher* pLucySearcher = new lucysearcher(this->m_pLog);

        //indexerThread* indexerThread = i.value();
        //pLucySearcher->open(indexerThread->getWorker()->getIndexer()->getDirectory());
        QString sDir2Index(i.key());
        pLucySearcher->open(sDir2Index);    //crash at the second search

        m_searchables[iIdx] = pLucySearcher->getSearcher();
        m_lucysearchables.append(pLucySearcher);
        iIdx++;
    }
    m_searchables[this->m_pIndexer->getIndexers()->size()] = nullptr;

    m_pMultiSearcher = new MultiSearcher(m_searchables);

    m_aquerys   = _CLNEW CL_NS(util)::ObjectArray<Query>(lpSearchinputs.length());
    m_query     = _CLNEW BooleanQuery();

//TODO: does * work?
    for(int i=0;i<lpSearchinputs.length();i++)
    {
        QPair<QString, QString> pSearchInput = lpSearchinputs.at(i);
        QString sSearchString = pSearchInput.first;
        QString sSearchField  = pSearchInput.second;
        m_query->add(QueryParser::parse(sSearchString.toStdWString().c_str(), sSearchField.toStdWString().c_str(), lucy::getNewAnalyzer()), true/*handle delete*/, BooleanClause::SHOULD);
    }

    m_sort = _CLNEW Sort(SortField::FIELD_SCORE()/*=? RELEVANCE*/);
    //TODO: set max hits
    //m_tophits = m_pMultiSearcher->_search(m_query, nullptr/*filter*/, 999/*max docs*/, m_sort);
    m_hits = m_pMultiSearcher->search(m_query, nullptr/*filter*/, m_sort);

    size_t iHits = (m_hits == nullptr ? 0 : m_hits->length());

    this->m_pLog->inf("hits:"+QString::number(iHits)+" query:"+QString::fromStdWString(m_query->toString(_T("text"))));

    return (int)iHits;
}

int searcher::getHitCount()
{
    //m_tophits->length();
    int iHits = (int)(m_hits == nullptr ? 0 : m_hits->length());
    return iHits;
}

QMap<QString, QString> searcher::GetHitAttrs(int iHitNr)
{
    QMap<QString, QString> mAttrs;
    Document* d = &(m_hits->doc(iHitNr));
    if(!d)return mAttrs;
    const Document::FieldsType* fields = d->getFields();
    Document::FieldsType::const_iterator fieldIt = fields->begin();
    while(fieldIt != fields->end())
    {
        Field* field = *fieldIt;
        QString sFieldName = QString::fromStdWString(field->name());
        QString sFieldValue= QString::fromStdWString(field->stringValue());

        //lastmodified, size

        if (mAttrs.contains(sFieldName))
            mAttrs[sFieldName] = mAttrs[sFieldName] + ", " + sFieldValue;
        else
            mAttrs[sFieldName] = sFieldValue;

        fieldIt++;
    }
    return mAttrs;
}

QString searcher::GetHitAttr(int iHitNr, QString sAttrName)
{
    if(!m_hits)return "";

    Document* d = &(m_hits->doc(iHitNr));
    if(!d)return "";
    const TCHAR* pAttrValue = d->get(sAttrName.toStdWString().c_str());

    if(!pAttrValue)
    {
        this->m_pLog->wrn("no "+sAttrName+" hitNr:"+QString::number(iHitNr)+"");
        return "";
    }
    return QString::fromStdWString(pAttrValue);
}

QString searcher::GetHitEnv(int iHitNr)
{
    if(!m_hits)return "";

    QueryScorer scorer(m_query);
    seacherHighlightFormatter hl_formatter;
    seacherHighlightFormatter* pHLFormatter = &hl_formatter;
    Formatter* pFormatter = pHLFormatter;
    Highlighter highlighter(pFormatter, &scorer);
    SimpleFragmenter frag(50);
    highlighter.setTextFragmenter(&frag);

    {
        const TCHAR* text = m_hits->doc(iHitNr).get(FIELDNAME_FULLTEXT);
        if(!text)return "";
        int maxNumFragmentsRequired     = 3;
        const TCHAR* fragmentSeparator  = _T("...");
        StringReader reader(text);
        TokenStream* tokenStream = this->m_lucysearchables.at(0)->getAnalyzer()->tokenStream(FIELDNAME_FULLTEXT, &reader);
        if(!tokenStream)return "";

        TCHAR* result =
                highlighter.getBestFragments(
                tokenStream,
                text,
                maxNumFragmentsRequired,
                fragmentSeparator);

        if(!result)return "";

        QString sHitEnv = QString::fromStdWString(result);

        _CLDELETE_CARRAY(result);
        _CLDELETE(tokenStream);

        sHitEnv.replace('\n', ' ').replace('\t', ' ').replace("  ", " ");

        return sHitEnv;
    }
}

void searcher::setEnv(logger* pLog, indexer* pIndexer)
{
    this->m_pLog    = pLog;
    this->m_pIndexer= pIndexer;
}
void searcher::cleanup(bool bConstructor)
{
    Q_UNUSED(bConstructor)
    if(m_aquerys)
    {
        _CLLDELETE(m_aquerys);
        m_aquerys = nullptr;
    }
    if(m_query)
    {
        delete m_query;
        m_query = nullptr;
    }
    if(m_hits)
    {
        delete m_hits;
        m_hits = nullptr;
    }
    if(m_tophits)
    {
        delete m_tophits;
        m_tophits = nullptr;
    }
    if(m_sort)
    {
        _CLDELETE(m_sort);
        m_sort = nullptr;
    }
    if(m_pMultiSearcher)
    {
        m_pMultiSearcher->close();
        delete m_pMultiSearcher;
        m_pMultiSearcher = nullptr;
    }

    if(m_searchables)_CLDELETE_ARRAY(m_searchables);
    if(this->m_aquerys)
    {
        this->m_aquerys->deleteValues();
        _CLLDELETE(this->m_aquerys);
        m_aquerys = nullptr;
    }

    for(int i=0;i<m_lucysearchables.length();i++)
    {
        m_lucysearchables.at(i)->setDirectory(nullptr);
        m_lucysearchables.at(i)->setSearcher(nullptr);
        delete m_lucysearchables.at(i);
    }
    m_lucysearchables.clear();
}

void searcher::sortBy(QString sSortFieldName, Qt::SortOrder order)
{
    if(!m_pMultiSearcher)
    {   //not yet searched...
        return;
    }

    if(m_hits)
    {
        delete m_hits;
        m_hits = nullptr;
    }
    if(m_tophits)
    {
        delete m_tophits;
        m_tophits = nullptr;
    }
    if(m_sort)
    {
        _CLDELETE(m_sort);
        m_sort = nullptr;
    }

    bool bReverse = ((order == Qt::AscendingOrder) ? true : false);

    //TODO: sort case insensitive (eg. with comparator)
    m_sort = _CLNEW Sort(new SortField(sSortFieldName.toStdWString().c_str(), SortField::STRING, bReverse));
    m_hits = m_pMultiSearcher->search(m_query, m_sort);
}

QSet<QString> searcher::fields()
{
    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
    QSet<QString> sf;

    //loop all indexes
    for(int i=0;i<sl.length();i++)
    {
        //const char* path = lucy::idxDir4Dir2Index(sl.at(i)).toLatin1().constData();
        IndexReader* reader = IndexReader::open(lucy::idxDir4Dir2Index(sl.at(i)).toLatin1().constData(), true/*closeDirectoryOnCleanup*/, nullptr);
        StringArrayWithDeletor fs(false);
        reader->getFieldNames(IndexReader::ALL, fs);
        size_t fsSize = fs.size();
        for (int n=0;n<fsSize;n++ )
        {
            QString sFieldName = QString::fromStdWString(fs[n]);
            sf.insert(sFieldName);
        }
        fs.clear();
        reader->close();
        _CLLDELETE(reader);
    }
    sf.insert("text");

    return sf;
}
