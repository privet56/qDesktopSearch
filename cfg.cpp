#include "cfg.h"
#include <QApplication>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QSettings>
#ifdef Q_WS_WIN
#include "windows.h"
#include "shlobj.h" //SHGetSpecialFolderPath
#endif

cfg::cfg(QObject *parent) : QObject(parent)
{
    m_sDefault = QString("");
    m_sPathName = QApplication::applicationFilePath() + ".cfg";
    m_pSettings = new QSettings(m_sPathName, QSettings::IniFormat, this);
}

QString cfg::getValue(QString sCfgEntryName)
{
    QString s = this->m_pSettings->value(sCfgEntryName, m_sDefault).toString();
    return replacePlaceHolders(s);
}
void cfg::setValue(QString sCfgEntryName, QString sCfgEntryValue)
{
    this->m_pSettings->setValue(sCfgEntryName, sCfgEntryValue);
    emit cfgChanged(sCfgEntryName, sCfgEntryValue);
}
QString cfg::replacePlaceHolders(QString s)
{
    if(s.indexOf('%') < 0)return s;

    //%MYFILES%,%TEMP%,%TMP%,%*APPDATA*%,%CURRENTDIR%(=AppDir)

    static QString sMYFILES = QStandardPaths::DocumentsLocation;//QDir::homePath();
/*#ifdef Q_WS_WIN
    static bool bMYFILES = false;
    if(!bMYFILES)
    {   bMYFILES = true;
        QSettings register("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat);
        QString allUserDesktop( register.value("Common Desktop").toString());
    }
#endif*/
    static QString sCURRENTDIR          = qApp->applicationDirPath();
    static QString sTEMP                = QDir::tempPath();
    static QString sPERSONAL_APPDATA    = QString(qgetenv("APPDATA"));
    static QString sCOMMON_APPDATA      = QString::null;
    static QString sAPP_PERSONAL_APPDATA= QString::null;
    static QString sAPP_COMMON_APPDATA  = QString::null;

    if(sPERSONAL_APPDATA.isNull())
    {
#ifdef Q_WS_WIN
        TCHAR lpBuffer[_MAX_PATH];      //CSIDL_* list -> see: http://msdn.microsoft.com/en-us/library/ms954149.aspx
        /*int ret = */SHGetSpecialFolderPath(NULL, lpBuffer, CSIDL_APPDATA, 0);
        sPERSONAL_APPDATA = QString::fromUtf16(lpBuffer);
#else
        sPERSONAL_APPDATA = QDir::homePath();
#endif
    }
    if(sCOMMON_APPDATA.isNull())
    {
#ifdef Q_WS_WIN
        TCHAR lpBuffer[_MAX_PATH];
        /*int ret = */SHGetSpecialFolderPath(NULL, lpBuffer, CSIDL_COMMON_APPDATA, 0);
        sCOMMON_APPDATA = QString::fromUtf16(lpBuffer);
#else
        sCOMMON_APPDATA = QDir::homePath();
#endif
        sAPP_COMMON_APPDATA = sCOMMON_APPDATA   + QDir::separator() + QCoreApplication::organizationName() + QDir::separator() + QCoreApplication::applicationName();
        cfg::mkdirfull(sAPP_COMMON_APPDATA);

        sAPP_PERSONAL_APPDATA = sPERSONAL_APPDATA+QDir::separator() + QCoreApplication::organizationName() + QDir::separator() + QCoreApplication::applicationName();
        cfg::mkdirfull(sAPP_PERSONAL_APPDATA);
    }

    s = s.replace("%MYFILES%"       ,sMYFILES);
    s = s.replace("%TEMP%"          ,sTEMP);
    s = s.replace("%TMP%"           ,sTEMP);
    s = s.replace("%CURRENTDIR%"    ,sCURRENTDIR);

    s = s.replace("%APPDATA%"               ,sPERSONAL_APPDATA);
    s = s.replace("%PERSONAL_APPDATA%"      ,sPERSONAL_APPDATA);
    s = s.replace("%COMMON_APPDATA%"        ,sCOMMON_APPDATA);
    s = s.replace("%APP_PERSONAL_APPDATA%"  ,sAPP_PERSONAL_APPDATA);
    s = s.replace("%APP_COMMON_APPDATA%"    ,sAPP_COMMON_APPDATA);

    return s;

}

bool cfg::mkdirfull(QString sDir)
{
    QDir d(sDir);
    return d.mkpath(sDir);
}
