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

        //7  = <b></b>
        //32 = <b><font color='red'></font></b>
        int len = (int)_tcslen(originalText) + 32;//7;
        TCHAR* ret = _CL_NEWARRAY(TCHAR, len + 1);
        _tcscpy(ret, _T("<b><font color='red'>"));
        //_tcscpy(ret, _T("<b>"));
        _tcscat(ret, originalText);
        _tcscat(ret, _T("</font></b>"));
        //_tcscat(ret, _T("</b>"));

        return ret;
    }
};

#endif // SEACHERHIGHLIGHTFORMATTER_H
