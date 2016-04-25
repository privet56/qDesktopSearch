#include "tokenizer.h"

tokenizer::tokenizer(CL_NS(util)::Reader* in) : CharTokenizer(in)
{

}

tokenizer::~tokenizer()
{

}

bool tokenizer::isTokenChar(const TCHAR c) const
{
    if (c == ' '    ||
        c == '\t'   ||
        c == '-'    ||
        c == '.'    ||
        c == '\n'   ||
        c == '\r'   ||
        c == ','    ||
        c == '/'    ||
        c == '"'    ||
        c == ':'    ||
        c == '\''   ||
         c == '\''   ||
        c == '<'    ||
        c == '>'    ||
        c <= 9)
    {
        return false;
    }

    QChar qc = QChar::fromLatin1(c);
    if(qc.isLetterOrNumber())
    {
        return true;
    }

    return false;
}

TCHAR tokenizer::normalize(const TCHAR c) const
{
    return c;
}
