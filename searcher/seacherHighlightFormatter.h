#ifndef SEACHERHIGHLIGHTFORMATTER_H
#define SEACHERHIGHLIGHTFORMATTER_H

#include "lucy.h"

class seacherHighlightFormatter : public Formatter
{
public:
    int numHighlights;

    explicit seacherHighlightFormatter();

    ~seacherHighlightFormatter()
    {

    }

    TCHAR* highlightTerm(const TCHAR* originalText, const TokenGroup* group)
    {
        if (group->getTotalScore() <= 0)
        {
            return STRDUP_TtoT(originalText);
        }
        numHighlights++; //update stats used in assertions

        int len = (int)_tcslen(originalText) + 7;
        TCHAR* ret = _CL_NEWARRAY(TCHAR, len + 1);
        _tcscpy(ret, _T("*"));
        _tcscat(ret, originalText);
        _tcscat(ret, _T("*"));

        return ret;
    }
};

#endif // SEACHERHIGHLIGHTFORMATTER_H
