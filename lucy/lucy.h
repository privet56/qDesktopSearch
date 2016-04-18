#ifndef LUCY_H
#define LUCY_H

#include <QObject>
#include "CLucene.h"
#include "CLucene/index/IndexModifier.h"
#include "logger.h"
#include "cfg.h"
#include "str.h"
#include <QApplication>

using namespace lucene;
using namespace lucene::analysis;
using namespace lucene::analysis::standard;
using namespace lucene::store;
using namespace lucene::index;
using namespace lucene::search;
using namespace lucene::document;

#define ID_FN _T("id_fn")
#define ID_FNANDDATE _T("id_fnanddate")

class lucy : public QObject
{
    Q_OBJECT
public:
    explicit lucy(logger* pLogger, QObject *parent = 0);
    ~lucy();
    void open();

protected:
    logger* m_pLogger;
    lucene::analysis::Analyzer* m_pAnalyzer;
    Directory* m_pDirectory;

signals:

public slots:
};

#endif // LUCY_H
