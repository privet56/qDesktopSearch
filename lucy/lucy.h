#ifndef LUCY_H
#define LUCY_H

#include <QObject>
#include "globalinclude.h"
#include "CLucene.h"
#include "CLucene/index/IndexModifier.h"
#include "CLucene/config/repl_wchar.h"
#include "CLucene/config/repl_tchar.h"
#include "CLucene/highlighter/Highlighter.h"
#include "CLucene/highlighter/QueryScorer.h"
#include "CLucene/highlighter/Highlighter.h"
#include "CLucene/highlighter/TokenGroup.h"
#include "CLucene/highlighter/SimpleHTMLFormatter.h"
#include "CLucene/highlighter/SimpleFragmenter.h"
#include "logger.h"
#include "cfg.h"
#include "str.h"
#include <QApplication>

#define OPTIMIZE_AFTER_INDEXED_FILES 1000000

using namespace lucene;
using namespace lucene::analysis;
using namespace lucene::analysis::standard;
using namespace lucene::store;
using namespace lucene::index;
using namespace lucene::search;
using namespace lucene::document;
using namespace lucene::queryParser;
using namespace lucene::util;

CL_NS_USE2(search, highlight);

CL_NS_USE(util)
CL_NS_USE(analysis)
CL_NS_USE(store)
CL_NS_USE(document)
CL_NS_USE(util)
CL_NS_USE2(analysis,standard)

#include "CLucene/_ApiHeader.h"
#include "CLucene/util/CLStreams.h"
#include "CLucene/util/Misc.h"
#include "CLucene/util/_bufferedstream.h"

struct DOC
{
    QString m_sAbsFileName;
    QString m_sid_fnanddate;
    int m_iDocNr;
};

class lucy : public QObject
{
    Q_OBJECT
public:
    explicit lucy(logger* pLogger, QObject *parent = 0);
    ~lucy();
    virtual void open(QString sDir2Index);
    virtual void close(bool bDeleteCompleteIndex=false);
    lucene::analysis::Analyzer* getAnalyzer();
    Directory* getDirectory();
    void setDirectory(Directory* pDirectory);
    static QString idxDir4Dir2Index(QString sDir2Index);
    static Analyzer* getNewAnalyzer();
    static QMutex* getIndexerLock();

protected:
    logger* m_pLogger;
    lucene::analysis::Analyzer* m_pAnalyzer;
    Directory* m_pDirectory;
    QString m_sDir2Index;
    QString m_sDirIndex;

signals:

public slots:
};

#endif // LUCY_H
