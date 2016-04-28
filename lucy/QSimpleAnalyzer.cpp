#include "QSimpleAnalyzer.h"
#include "QTokenizer.h"

QSimpleAnalyzer::QSimpleAnalyzer() : ANALYZERBASE()
{

}

TokenStream* QSimpleAnalyzer::tokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader)
{
    ///*TokenStream* tokenStreamBase = */ANALYZERBASE::tokenStream(fieldName, reader);
    return _CLNEW QTokenizer(reader);
}

TokenStream* QSimpleAnalyzer::reusableTokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader)
{
    ///*TokenStream* tokenStreamBase = */ANALYZERBASE::reusableTokenStream(fieldName, reader);

    Tokenizer* tok = static_cast<Tokenizer*>(getPreviousTokenStream());

     if (tok == NULL)
     {
         tok = _CLNEW QTokenizer(reader);
         setPreviousTokenStream(tok);
     }
     else
     {
         tok->reset(reader);
     }
     return tok;
}

QSimpleAnalyzer::~QSimpleAnalyzer()
{
    //TODO: fix crash in ~StandardAnalyzer (it destroys sometimes a destroyed filteredTokenStream)
    //currently in ANALYZERBASE::~ANALYZERBASE 2 lines are deactivated...
}
