#ifndef QSIMPLEANALYZER_H
#define QSIMPLEANALYZER_H

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

#define ANALYZERBASE Analyzer               //his problems: not case-insensitive, no stopWords
//#define ANALYZERBASE StandardAnalyzer

//TODO: crash in ~StandardAnalyzer if base is StandardAnalyzer, check what is better to use as super: Analyzer or StandardAnalyzer(has StopFilter builtin)
class QSimpleAnalyzer : public ANALYZERBASE
{
public:
    QSimpleAnalyzer();
    ~QSimpleAnalyzer();

    TokenStream* tokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);
    TokenStream* reusableTokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);

};

#endif // QSIMPLEANALYZER_H
