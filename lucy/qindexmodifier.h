#ifndef QINDEXMODIFIER_H
#define QINDEXMODIFIER_H

#include "globalinclude.h"
#include "lucy.h"

class QIndexModifier : public IndexModifier
{
public:
    QIndexModifier(CL_NS(store)::Directory* directory, CL_NS(analysis)::Analyzer* analyzer, bool create);
    IndexReader* getIndexReader();

    int32_t deleteDocuments(Term* term);
    void deleteDocument(int32_t docNum);
    void flush();

protected:
    bool m_bDocDeletionCalled;
};

#endif // QINDEXMODIFIER_H
