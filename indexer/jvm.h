#ifndef JVM_H
#define JVM_H

#include <QObject>
#include <QApplication>
#include <QLibrary>
#include "logger.h"
#include "java/include/jni.h"

class jvm : public QObject
{
    Q_OBJECT
public:
    explicit jvm(logger* pLogger, QObject *parent = 0);
    ~jvm();
    bool load();
    bool loadClasses(QString sJvmDll, QString sJavaClassPath);

protected:
    void cmp(char* p1, char* p2);
    QString js2qs(jstring js);
    jclass getClass(QString sClassAbsName);
    jobject getObject(QString sClassAbsName);
    jobject getObject(QString sClassAbsName, QString sStringConstructorParameter);
    jobject getObject(QString sClassAbsName, jobject constrParam, QString sConstrParamClassName, bool bErrIfCannotInit=true);
    void extractMetaContents(jobject oMetadata, jclass cMetadata, jstring metaName, QMap<QString, QStringList>* pMetas, QString sMetaName);

protected:
    logger* m_pLogger;
    JNIEnv* m_pEnv;
    JavaVM* m_pVM;

    QMap<QString, jclass> m_classes;

signals:

public slots:
    void getMetaContents(QString sAbsPathName, QMap<QString, QStringList>* pMetas);

};

#endif // JVM_H
