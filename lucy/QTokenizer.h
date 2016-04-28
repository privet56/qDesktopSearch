#ifndef QTOKENIZER_H
#define QTOKENIZER_H

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

using namespace lucene;
using namespace lucene::analysis;
using namespace lucene::analysis::standard;

class QTokenizer : public CharTokenizer
{
public:
    QTokenizer(CL_NS(util)::Reader* in, bool deleteReader=false);
    ~QTokenizer();

    bool isTokenChar(const TCHAR c) const;
    TCHAR normalize(const TCHAR c) const;

};

#endif // QTOKENIZER_H
