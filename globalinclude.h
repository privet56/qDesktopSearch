#ifndef GLOBALINCLUDE_H
#define GLOBALINCLUDE_H

#include <QString>

//TODO: check if this is necessary for porterStemmer
//#define S2TCHAR(s){(TCHAR*)s.toStdWString().c_str()}

#define FIELDNAME_FULLTEXT      _T("text")
#define FIELDNAME_FILENAME      _T("filename")
#define FIELDNAME_ABSPATHNAME   _T("pathname")
#define ID_FN                   _T("id_fn")
#define ID_FNANDDATE            _T("id_fnanddate")
#define DIRS2INDEX QString("dirs2index")

#define SINGLEINSTANCE_QDESKTOPSEARCH_WAKEUP QString("SINGLEINSTANCE_QDESKTOPSEARCH_WAKEUP")

#endif // GLOBALINCLUDE_H
