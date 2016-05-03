#include "qindexmodifier.h"

QIndexModifier::QIndexModifier(CL_NS(store)::Directory* directory, CL_NS(analysis)::Analyzer* analyzer, bool create) : IndexModifier(directory, analyzer, create), m_bDocDeletionCalled(false)
{

}

IndexReader* QIndexModifier::getIndexReader()
{
    if(!this->indexReader)
    {
        this->createIndexReader();
    }
    return this->indexReader;
}

int32_t QIndexModifier::deleteDocuments(Term* term)
{
    int32_t iDeletedDocs = IndexModifier::deleteDocuments(term);
    if(iDeletedDocs > 0)
    {
        m_bDocDeletionCalled = true;
    }
    return iDeletedDocs;
}

void QIndexModifier::deleteDocument(int32_t docNum)
{
    m_bDocDeletionCalled = true;
    IndexModifier::deleteDocument(docNum);
}
void QIndexModifier::flush()
{
    if(m_bDocDeletionCalled)
    {
        this->getIndexReader()->commit();
        m_bDocDeletionCalled = false;
    }
    IndexModifier::flush();
}
