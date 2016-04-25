#ifndef ANALYZER_H
#define ANALYZER_H

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

class analyzer : public StandardAnalyzer
{
public:
    analyzer();
    ~analyzer();

    TokenStream* tokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);
    TokenStream* reusableTokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);

};

#endif // ANALYZER_H
