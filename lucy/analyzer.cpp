#include "analyzer.h"
#include "tokenizer.h"

analyzer::analyzer()
{

}

TokenStream* analyzer::tokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader)
{
    return _CLNEW tokenizer(reader);
}

TokenStream* analyzer::reusableTokenStream(const TCHAR* fieldName, CL_NS(util)::Reader* reader)
{
    Tokenizer* tok = static_cast<Tokenizer*>(getPreviousTokenStream());

     if (tok == NULL)
     {
         tok = _CLNEW tokenizer(reader);
         setPreviousTokenStream(tok);
     }
     else
     {
         tok->reset(reader);
     }
     return tok;
}

analyzer::~analyzer()
{

}
