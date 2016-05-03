#include "jvm.h"
#include <QFile>
#include <QDebug>
#include <QThread>
#include "str.h"
#include "f.h"

#ifdef Q_OS_WIN
    #include <windows.h>
#elif Q_OS_UNIX
    #include <unistd.h>
#elif Q_OS_MAC
    #include <stdio.h>
    #include <stdint.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif

typedef jint (JNICALL* JNI_CreateJavaVM2) (JavaVM **pvm, void **penv, void *args);

#define OPTIONSLEN 4

jvm::jvm(logger* pLogger, QObject *parent) : QObject(parent), m_pLogger(pLogger), m_pEnv(nullptr), m_pVM(nullptr)
{

}

bool jvm::load()
{
#ifdef _DEBUG
    return true;
#endif

    JavaVMInitArgs vm_args;
    JavaVMOption options[OPTIONSLEN];

    //TODO: better list all jars in a dir and set all as class.path
    QString sTika(this->m_pLogger->GetCfg()->getValue("tika"));
    if(!QFile::exists(sTika))
        return this->m_pLogger->err("fnf "+sTika);

    QString sJavaClassPath("-Djava.class.path="+sTika);
    QByteArray ba = sJavaClassPath.toLocal8Bit();
    char* pJavaClassPath = (char*)strdup(ba.constData());
    options[0].optionString = (char*)pJavaClassPath;
    options[1].optionString = (char*)"-Dworkdir=.";
    QString sXMX = getMemSetting4Jvm();
    QString sXmx("-Xmx"+sXMX+"m");
    QString sXms("-Xms"+sXMX+"m");
    ba = sXmx.toLocal8Bit();
    char* pXmx = (char*)strdup(ba.constData());
    ba = sXms.toLocal8Bit();
    char* pXms = (char*)strdup(ba.constData());
    options[2].optionString = (char*)pXms;
    options[3].optionString = (char*)pXmx;

    QString sJvmDll(qApp->applicationDirPath() + "/jre/bin/server/jvm.dll");    //TODO: .so for !win
    if(!QFile::exists(sJvmDll))
        return this->m_pLogger->err("fnf "+sJvmDll);

    QLibrary lib(sJvmDll, this);                                                //HMODULE jvmdll = LoadLibrary(sJvmDll);

    if(!lib.load())
        return this->m_pLogger->err("cannot load jvm("+sJvmDll+")");

    vm_args.version             = JNI_VERSION_1_6;
    vm_args.options             = options;
    vm_args.nOptions            = OPTIONSLEN;
    vm_args.ignoreUnrecognized  = JNI_TRUE;

    JNI_CreateJavaVM2 CreateJavaVM = (JNI_CreateJavaVM2)lib.resolve("JNI_CreateJavaVM");

    jint res = CreateJavaVM(&m_pVM, (void **)&m_pEnv, &vm_args);

    if (res < 0)
        return this->m_pLogger->err("cannot load jvm("+sJvmDll+")");

    this->m_pLogger->inf("pJavaClassPath:"+QString(pJavaClassPath));

    {
        free(pJavaClassPath);pJavaClassPath = nullptr;
        free(pXms);pXms = nullptr;
        free(pXmx);pXmx = nullptr;
    }

    if(!getClass("java.io.File") ||
       !getClass("org.apache.tika.parser.AutoDetectParser") ||
       !getClass("org.apache.tika.sax.BodyContentHandler") ||
       !getClass("org.apache.tika.metadata.Metadata") ||
       !getClass("org.apache.tika.parser.ParseContext") ||
       !getClass("java.io.FileInputStream") ||
       !getClass("org.apache.tika.parser.AutoDetectParser"))
    {
        return false;
    }
    //this->m_pLogger->inf("tika loaded: "+sJavaClassPath+"");
    return true;
}

void jvm::cmp(char* p1, char* p2)
{
    printf("\n---1:");
    for (int i = 0; i < strlen(p1); i++)
    {
        printf("<%c>", p1[i]);
    }
    printf("\n---2:");
    for (int i = 0; i < strlen(p2); i++)
    {
        printf("<%c>", p2[i]);
    }
    printf("\n---");
}

jclass jvm::getClass(QString sClassAbsName)
{
    QString sClassAbsNameWithDots(sClassAbsName);
    QString sClassAbsNameWithSlash(sClassAbsName);
    sClassAbsNameWithDots.replace('/', '.');
    sClassAbsNameWithSlash.replace('.', '/');

    jclass jclass2load = m_classes[sClassAbsNameWithSlash];
    if(jclass2load) return jclass2load;
    jclass2load = m_classes[sClassAbsNameWithDots];
    if(jclass2load) return jclass2load;

    {
        /*
        size_t const l = sClassAbsName.length();
        char* args = new char[l + 1];
        char* args2 = new char[l + 1];
        ::memset(args, 0, l+1);
        ::memset(args2, 0, l+1);
        strncpy(args, "java/io/File", l);
        strncpy(args2, sClassAbsNameWithSlash.toLatin1().data(), l);

        cmp(args, args2);

        jclass2load = m_pEnv->FindClass(args2);
        delete[] args;args = nullptr;
        delete[] args2;args2 = nullptr;
        //return (jclass)this->m_pLogger->err("cannot load class0 >"+QString(args)+"<");
        */
        jclass2load = m_pEnv->FindClass(sClassAbsNameWithSlash.toLatin1().data());
        if(!jclass2load)
            return (jclass)this->m_pLogger->err("cannot load class1 "+sClassAbsNameWithSlash);

        //jclass2load = (jclass)m_pEnv->NewGlobalRef(jclass2load);  //does not help

        m_classes[sClassAbsName]            = jclass2load;
        m_classes[sClassAbsNameWithDots]    = jclass2load;
        m_classes[sClassAbsNameWithSlash]   = jclass2load;
    }
    return jclass2load;
}
jobject jvm::getObject(QString sClassAbsName, jobject constrParam, QString sConstrParamClassName, bool bErrIfCannotInit)
{
    sConstrParamClassName.replace('.','/');

    jclass c = this->getClass(sClassAbsName);
    if(!c)return (jobject)this->m_pLogger->err("cannot load class2 "+sClassAbsName);
    jmethodID init = m_pEnv->GetMethodID(c, "<init>", QString("(L"+sConstrParamClassName+";)V").toLatin1().data());
    if(!init)return (jobject)this->m_pLogger->err("cannot load class3<init> "+sClassAbsName+"    param:"+sConstrParamClassName);
    jobject o = m_pEnv->NewObject(c, init, constrParam);
    if(!o && bErrIfCannotInit)return (jobject)this->m_pLogger->err("cannot load class4<init>.NewObject "+sClassAbsName+"("+sConstrParamClassName+")");
    return o;
}
jobject jvm::getObject(QString sClassAbsName)
{
    jclass c = this->getClass(sClassAbsName);
    if(!c)return (jobject)this->m_pLogger->err("cannot load class5 "+sClassAbsName);
    jmethodID init = m_pEnv->GetMethodID(c, "<init>", "()V");
    if(!init)return (jobject)this->m_pLogger->err("cannot load class6<init> "+sClassAbsName);
    jobject o = m_pEnv->NewObject(c, init);
    if(!o)return (jobject)this->m_pLogger->err("cannot load class7<init>.NewObject "+sClassAbsName);
    return o;
}

jobject jvm::getObject(QString sClassAbsName, QString sStringConstructorParameter)
{
    jclass c = this->getClass(sClassAbsName);
    if(!c)return (jobject)this->m_pLogger->err("cannot load class8 "+sClassAbsName);
    jmethodID init = m_pEnv->GetMethodID(c, "<init>", "(Ljava/lang/String;)V");
    if(!init)return (jobject)this->m_pLogger->err("cannot load class9<init> "+sClassAbsName+"    param:"+sStringConstructorParameter);
    jstring js = m_pEnv->NewStringUTF(sStringConstructorParameter.toLatin1().data());
    jobject o = sStringConstructorParameter.isNull() ? m_pEnv->NewObject(c, init) : m_pEnv->NewObject(c, init, js);
    m_pEnv->DeleteLocalRef(js);
    if(!o)return (jobject)this->m_pLogger->err("cannot load class10<init>.NewObject "+sClassAbsName);
    return o;
}

void jvm::extractMetaContents(jobject oMetadata, jclass cMetadata, jstring metaName, QMap<QString, QStringList>* pMetas, QString sMetaName)
{
    jmethodID getValues = m_pEnv->GetMethodID(cMetadata, "getValues", "(Ljava/lang/String;)[Ljava/lang/String;");
    if(!getValues){this->m_pLogger->err("cannot extract getValues");return;}
    jobjectArray array = (jobjectArray)m_pEnv->CallObjectMethod(oMetadata, getValues, metaName);
    if(!array){this->m_pLogger->err("cannot extract array");return;}
    jsize const length = m_pEnv->GetArrayLength(array);

    if(length < 1){this->m_pLogger->wrn("no vals for meta "+sMetaName);return;}

    sMetaName = sMetaName.toLower().trimmed();
    QStringList sl;

    for(jsize index(0); index < length; ++index)
    {
        jstring element = (jstring)m_pEnv->GetObjectArrayElement(array, index);
        QString sMetaValue(js2qs(element));
        m_pEnv->DeleteLocalRef(element);
        sl << sMetaValue;
    }

    m_pEnv->DeleteLocalRef(array);

    bool bContains = pMetas->contains(sMetaName);

    if(bContains)
    {
        pMetas->operator [](sMetaName) << sl;
    }
    else
    {
        pMetas->operator [](sMetaName) = sl;
    }
}

/*
    import java.io.File;
    import java.io.FileInputStream;
    import java.io.IOException;

    import org.apache.tika.exception.TikaException;
    import org.apache.tika.metadata.Metadata;
    import org.apache.tika.parser.AutoDetectParser;
    import org.apache.tika.parser.ParseContext;
    import org.apache.tika.parser.Parser;
    import org.apache.tika.sax.BodyContentHandler;

    import org.xml.sax.SAXException;

      File file = new File("boy.jpg");

      //Parser method parameters
      Parser parser = new AutoDetectParser();
      BodyContentHandler handler = new BodyContentHandler();
      Metadata metadata = new Metadata();
      FileInputStream inputstream = new FileInputStream(file);
      ParseContext context = new ParseContext();

      parser.parse(inputstream, handler, metadata, context);

      //getting the list of all meta data elements
      String[] metadataNames = metadata.names();

      for(String name : metadataNames) {
         System.out.println(name + ": " + metadata.get(name));
      }
*/
void jvm::getMetaContents(QString sAbsPathName, QMap<QString, QStringList>* pMetas)
{
    bool isGuiThread = QThread::currentThread() == QCoreApplication::instance()->thread();
    if (!isGuiThread)
    {
        //http://stackoverflow.com/questions/19113719/jni-findclass-function-returns-null
        //http://developer.android.com/training/articles/perf-jni.html#faq_FindClass
        this->m_pLogger->err("call jvm on the main thread!");
        return;
    }

    /*//TODO: check java exceptions ala
    jthrowable exc = env->ExceptionOccurred();
     if(exc)
     {
       env->ExceptionDescribe();
       env->ExceptionClear();
     }*/

    sAbsPathName = str::normalizePath(sAbsPathName, false);

#ifdef _DEBUG
    QStringList sl;
    QString sTxt = sAbsPathName;
    //if (sTxt.toLower().endsWith(".txt"))
    sTxt += " " + f::getFC(sAbsPathName);
    sl << QString(sTxt);
    pMetas->operator []("text") = sl;
    return;
#endif

//get objects
    jobject oFile               = getObject("java.io.File", sAbsPathName);
    jobject oFileInputStream    = getObject("java.io.FileInputStream", oFile, "java.io.File", false);
    if(!oFileInputStream)       //almost normal case, if file is exclusively opened by another process...
    {
        m_pEnv->DeleteLocalRef(oFile);
        this->m_pLogger->wrn("cannot load file{"+sAbsPathName+"}");return;
    }
    jobject oAutoDetectParser   = getObject("org.apache.tika.parser.AutoDetectParser");
    jobject oMetadata           = getObject("org.apache.tika.metadata.Metadata");
    jobject oParseContext       = getObject("org.apache.tika.parser.ParseContext");
    jobject oBodyContentHandler = getObject("org.apache.tika.sax.BodyContentHandler");
//call parse
    {
        jclass cAutoDetectParser = this->getClass("org.apache.tika.parser.AutoDetectParser");//m_pEnv->GetObjectClass(oAutoDetectParser);
        QString sSignature("("
                           "Ljava/io/InputStream;"
                           "Lorg/xml/sax/ContentHandler;"
                           "Lorg/apache/tika/metadata/Metadata;"
                           "Lorg/apache/tika/parser/ParseContext;"
                           ")V");
        jmethodID parse = m_pEnv->GetMethodID(cAutoDetectParser, "parse", sSignature.toLatin1().constData());
        if(!parse)
        {
            this->m_pLogger->err("cannot load parse("+sSignature+")");return;
        }
        //TODO: check java exceptions!
        m_pEnv->CallVoidMethod(oAutoDetectParser, parse, oFileInputStream, oBodyContentHandler, oMetadata, oParseContext);
    }
//extract fulltext
    {
        jclass cBodyContentHandler = this->getClass("org.apache.tika.sax.BodyContentHandler");//m_pEnv->GetObjectClass(oBodyContentHandler);
        jmethodID toString = m_pEnv->GetMethodID(cBodyContentHandler, "toString", "()Ljava/lang/String;");
        if(!toString)
        {
            this->m_pLogger->err("cannot load toString{()Ljava/lang/String;}");return;
        }
        jstring js = (jstring)m_pEnv->CallObjectMethod(oBodyContentHandler, toString);
        QString sText(js2qs(js));
        m_pEnv->DeleteLocalRef(js);
        if(!str::isempty(sText, true))
        {
            QStringList sl;
            sl << sText;
            pMetas->operator []("text") = sl;
        }
    }
//extract metas
    {
        jclass cMetadata    = getClass("org.apache.tika.metadata.Metadata");//m_pEnv->GetObjectClass(oMetadata);
        jmethodID names     = m_pEnv->GetMethodID(cMetadata, "names", "()[Ljava/lang/String;");
        if(!names){this->m_pLogger->err("cannot load names");return;}
        jobjectArray array  = (jobjectArray)m_pEnv->CallObjectMethod(oMetadata, names);
        if(!array){this->m_pLogger->err("cannot load array");return;}
        jsize const length  = m_pEnv->GetArrayLength(array);

        for(jsize index(0); index < length; ++index)
        {
            jstring element = (jstring) m_pEnv->GetObjectArrayElement(array, index);
            QString sMetaName(js2qs(element));
            extractMetaContents(oMetadata, cMetadata, element, pMetas, sMetaName);
            m_pEnv->DeleteLocalRef(element);
        }
        {   //call close on oFileInputStream
            jclass cFileInputStream = getClass("java.io.FileInputStream");
            jmethodID close = m_pEnv->GetMethodID(cFileInputStream, "close", "()V");
            if(!close)
            {
                this->m_pLogger->err("cannot load close{()V}");
            }
            m_pEnv->CallVoidMethod(oFileInputStream, close);
        }

        m_pEnv->DeleteLocalRef(array);
//cleanup before return
        {
            m_pEnv->DeleteLocalRef(oFile);
            m_pEnv->DeleteLocalRef(oAutoDetectParser);
            m_pEnv->DeleteLocalRef(oBodyContentHandler);
            m_pEnv->DeleteLocalRef(oMetadata);
            m_pEnv->DeleteLocalRef(oParseContext);
            m_pEnv->DeleteLocalRef(oFileInputStream);
        }
    }
}

QString jvm::js2qs(jstring js)
{
    QString s(m_pEnv->GetStringUTFChars(js, 0));
    return s;
    /*
    char const* nativeString    = m_pEnv->GetStringUTFChars(js, 0);
    size_t const  nativeLength  = strlen(nativeString);
    char* args = new char[nativeLength + 1];
    ::memset(args, 0, nativeLength+1);
    strncpy(args, nativeString, nativeLength);
    QString s(args);
    m_pEnv->ReleaseStringUTFChars(js, nativeString);
    delete[] args;
    args = nullptr;
    return s;
    */
}

QString jvm::getMemSetting4Jvm()
{
    QString sXMXCfg(this->m_pLogger->GetCfg()->getValue("xmx"));
    if(!str::isempty(sXMXCfg))
    {
        return sXMXCfg;
    }

    int iTotalRAMinMB = 10240;//default: 10 GB

    //TODO: test non-windows code

#ifdef Q_OS_WIN
    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memory_status))
    {
        iTotalRAMinMB = memory_status.ullTotalPhys / (1024 * 1024);
    }
#elif Q_OS_UNIX
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    long ram = pages * page_size;
    iTotalRAMinMB = ram / (1024 * 1024);
#elif Q_OS_MAC
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    u_int namelen = sizeof(mib) / sizeof(mib[0]);
    uint64_t size;
    size_t len = sizeof(size);

    if (sysctl(mib, namelen, &size, &len, NULL, 0) < 0)
    {
        //perror("sysctl");
    }
    else
    {
        iTotalRAMinMB = size / (1024 * 1024);    //printf("HW.HW_MEMSIZE = %llu bytes\n", size);
    }
#endif

    if (iTotalRAMinMB < 1024)
        iTotalRAMinMB = 1024;

    int iRAMinMB = iTotalRAMinMB / 10;  //I would like to have 1/10 of your available memory for the jvm

    if (iRAMinMB < 512)
        iRAMinMB = 512;
    if (iRAMinMB > 2048)
        iRAMinMB = 2048;

    QString sXMX = QString::number(iRAMinMB);
    return sXMX;
}

jvm::~jvm()
{
    //(*myJVM)->DestroyJavaVM(myJVM);       //TODO:
}
