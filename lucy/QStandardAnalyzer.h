#ifndef QSTANDARDANALYZER
#define QSTANDARDANALYZER

#include "lucy.h"
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

#include "CLucene/analysis/AnalysisHeader.h"

/**
* Filters
	{@link QTokenizer} with
	{@link lucene::analysis::standard::StandardFilter}, 
	{@link lucene::analysis::LowerCaseFilter} and
	{@link lucene::analysis::StopFilter}, using a list of English stop words.
*/

class QStandardAnalyzer : public Analyzer
{
public:
    /** Builds an analyzer.*/
    QStandardAnalyzer();

    /** Builds an analyzer with the given stop words. */
    QStandardAnalyzer(const TCHAR** stopWords);

    /** Builds an analyzer with the stop words from the given file.
    * @see WordlistLoader#getWordSet(File)
    */
    QStandardAnalyzer(const char* stopwordsFile, const char* enc = NULL);

    /** Builds an analyzer with the stop words from the given reader.
    * @see WordlistLoader#getWordSet(Reader)
    */
    QStandardAnalyzer(CL_NS(util)::Reader* stopwordsReader, const bool _bDeleteReader = false);

    virtual ~QStandardAnalyzer();

private:
	CLTCSetList* stopSet;
	int32_t maxTokenLength;

    /** Default maximum allowed token length */
    LUCENE_STATIC_CONSTANT(int32_t, DEFAULT_MAX_TOKEN_LENGTH = 255);

    class QSavedStreams;

public:

	/**
	* Constructs a QTokenizer filtered by a 
	* StandardFilter, a LowerCaseFilter and a StopFilter.
	*/
	TokenStream* tokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);

	TokenStream* reusableTokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader);

	/**
	* Set maximum allowed token length.  If a token is seen
	* that exceeds this length then it is discarded.  This
	* setting only takes effect the next time tokenStream or
	* reusableTokenStream is called.
	*/
	void setMaxTokenLength(const int32_t length) {
		maxTokenLength = length;
	}

	/**
	* @see #setMaxTokenLength
	*/
	int getMaxTokenLength() const {
		return maxTokenLength;
	}
};

#endif	//QSTANDARDANALYZER
