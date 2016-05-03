#include "lucysearcher.h"
#include "lucy.h"

lucysearcher::lucysearcher(logger* pLogger, QObject *parent) : lucy(pLogger, parent), m_pIndexSearcher(nullptr)
{

}

lucysearcher::~lucysearcher()
{
    if(m_pIndexSearcher)
    {
        m_pIndexSearcher->close();
        delete m_pIndexSearcher;
    }
    m_pIndexSearcher = nullptr;
}

void lucysearcher::open(Directory* pDirectory)
{
    m_pIndexSearcher = new lucene::search::IndexSearcher(pDirectory);
}

void lucysearcher::open(QString sDir2Index, IndexReader* pReader)
{
    lucy::open(sDir2Index);

    if(pReader == nullptr)
    {
        open(this->m_pDirectory);
    }
    else
    {
        m_pIndexSearcher = new lucene::search::IndexSearcher(pReader);
    }
}

IndexSearcher* lucysearcher::getSearcher()
{
    return this->m_pIndexSearcher;
}
void lucysearcher::setSearcher(IndexSearcher* pSearcher)
{
    this->m_pIndexSearcher = pSearcher;
}
