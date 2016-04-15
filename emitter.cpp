#include "emitter.h"

emitter::emitter(QObject *parent) : QObject(parent)
{

}
void emitter::emitlog(QString s, logger::LogLevel level, QObject* pLogTarget)
{
    connect(this,SIGNAL(log(QString,logger::LogLevel)),pLogTarget,SLOT(log(QString,logger::LogLevel)));
    emit log(s,level);
}
