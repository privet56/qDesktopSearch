#include "CLucene/_ApiHeader.h"
#include "QStandardAnalyzer.h"

#include "CLucene/util/CLStreams.h"
#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/analysis/Analyzers.h"
#include "CLucene/analysis/standard/StandardFilter.h"
#include "QTokenizer.h"

#include "QStandardTokenizer.h"

CL_NS_USE(util)
CL_NS_USE(analysis)

QStandardAnalyzer::QStandardAnalyzer():
	stopSet(_CLNEW CLTCSetList(true)), maxTokenLength(DEFAULT_MAX_TOKEN_LENGTH)
{
  StopFilter::fillStopTable( stopSet,CL_NS(analysis)::StopAnalyzer::ENGLISH_STOP_WORDS);
}

QStandardAnalyzer::QStandardAnalyzer( const TCHAR** stopWords):
	stopSet(_CLNEW CLTCSetList(true)), maxTokenLength(DEFAULT_MAX_TOKEN_LENGTH)
{
	StopFilter::fillStopTable( stopSet,stopWords );
}

QStandardAnalyzer::QStandardAnalyzer(const char* stopwordsFile, const char* enc):
	stopSet(_CLNEW CLTCSetList(true)), maxTokenLength(DEFAULT_MAX_TOKEN_LENGTH)
{
    if (enc == NULL)
		enc = "ASCII";
	WordlistLoader::getWordSet(stopwordsFile, enc, stopSet);
}

QStandardAnalyzer::QStandardAnalyzer(CL_NS(util)::Reader* stopwordsReader, const bool _bDeleteReader):
	stopSet(_CLNEW CLTCSetList(true)), maxTokenLength(DEFAULT_MAX_TOKEN_LENGTH)
{
	WordlistLoader::getWordSet(stopwordsReader, stopSet, _bDeleteReader);
}

class QStandardAnalyzer::QSavedStreams : public TokenStream
{
public:
    QTokenizer* tokenStream;
    TokenStream* filteredTokenStream;

    QSavedStreams() : tokenStream(NULL), filteredTokenStream(NULL)
    {

    }

    void close(){}

    Token* next(Token* token)
    {
        Q_UNUSED(token)
        return NULL;
    }
};

QStandardAnalyzer::~QStandardAnalyzer()
{
    QSavedStreams* t = reinterpret_cast<QSavedStreams*>(this->getPreviousTokenStream());
	if (t) _CLDELETE(t->filteredTokenStream);
	_CLLDELETE(stopSet);
}

TokenStream* QStandardAnalyzer::tokenStream(const TCHAR* /*fieldName*/, Reader* reader)
{
	BufferedReader* bufferedReader = reader->__asBufferedReader();
	TokenStream* ret;

    if(bufferedReader == NULL)
        ret = _CLNEW QTokenizer(_CLNEW FilteredBufferedReader(reader, false), true);
	else
        ret = _CLNEW QTokenizer(bufferedReader);
	//ret->setMaxTokenLength(maxTokenLength);
	ret = _CLNEW StandardFilter(ret,true);
	ret = _CLNEW LowerCaseFilter(ret,true);
	ret = _CLNEW StopFilter(ret,true, stopSet);
	return ret;
}

TokenStream* QStandardAnalyzer::reusableTokenStream(const TCHAR* fieldName, Reader* reader)
{
    Q_UNUSED(fieldName)     //TODO: act differently in QTokenizer, depending on (nr,str,bool)fieldType

    QSavedStreams* streams = reinterpret_cast<QSavedStreams*>(getPreviousTokenStream());
    if (streams == NULL)
    {
        streams = _CLNEW QSavedStreams();
		setPreviousTokenStream(streams);

		BufferedReader* bufferedReader = reader->__asBufferedReader();
        if (bufferedReader == NULL)
            streams->tokenStream = _CLNEW QTokenizer(_CLNEW FilteredBufferedReader(reader, false), true);
		else
            streams->tokenStream = _CLNEW QTokenizer(bufferedReader);

		streams->filteredTokenStream = _CLNEW StandardFilter(streams->tokenStream, true);
		streams->filteredTokenStream = _CLNEW LowerCaseFilter(streams->filteredTokenStream, true);
		streams->filteredTokenStream = _CLNEW StopFilter(streams->filteredTokenStream, true, stopSet);
    }
    else
    {
		streams->tokenStream->reset(reader);
	}

	//streams->tokenStream->setMaxTokenLength(maxTokenLength);

	return streams->filteredTokenStream;
}
