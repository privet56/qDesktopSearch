#ifndef LUCYSEARCHER_H
#define LUCYSEARCHER_H

#include <QObject>
#include "lucy.h"

class lucysearcher : public lucy
{
public:
    lucysearcher(logger* pLogger, QObject *parent = 0);

protected:
    IndexSearcher* m_pIndexSearcher;

};

#endif // LUCYSEARCHER_H
