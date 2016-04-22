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

void lucysearcher::open(QString sDir2Index)
{
    lucy::open(sDir2Index);

    open(this->m_pDirectory);
}

IndexSearcher* lucysearcher::getSearcher()
{
    return this->m_pIndexSearcher;
}
void lucysearcher::setSearcher(IndexSearcher* pSearcher)
{
    this->m_pIndexSearcher = pSearcher;
}
