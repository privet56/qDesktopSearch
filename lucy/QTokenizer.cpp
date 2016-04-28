#include "QTokenizer.h"

QTokenizer::QTokenizer(CL_NS(util)::Reader* in, bool deleteReader/*=false*/) : CharTokenizer(in)
{

}

QTokenizer::~QTokenizer()
{

}

bool QTokenizer::isTokenChar(const TCHAR c) const
{
    if (c == 0      ||
        c == ' '    ||
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

TCHAR QTokenizer::normalize(const TCHAR c) const
{
    return c;
}
