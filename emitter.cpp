#include "emitter.h"

emitter::emitter(QObject* pLogTarget, QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(log(QString,logger::LogLevel)),pLogTarget,SLOT(log(QString,logger::LogLevel)));
}
void emitter::emitlog(QString s, logger::LogLevel level, QObject* pLogTarget)
{
    Q_UNUSED(pLogTarget)
    emit log(s,level);
}
