#ifndef LUCYSEARCHER_H
#define LUCYSEARCHER_H

#include <QObject>
#include "lucy.h"

class lucysearcher : public lucy
{
public:
    lucysearcher(logger* pLogger, QObject *parent = 0);
    ~lucysearcher();
    void open(QString sDir2Index, IndexReader* pReader);
    void open(Directory* pDirectory);
    IndexSearcher* getSearcher();
    void setSearcher(IndexSearcher* pSearcher);

protected:
    IndexSearcher* m_pIndexSearcher;

};

#endif // LUCYSEARCHER_H
