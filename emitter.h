#ifndef EMITTER_H
#define EMITTER_H

#include <QObject>
#include "logger.h"

class emitter : public QObject
{
    Q_OBJECT
public:
    explicit emitter(QObject* pLogTarget, QObject *parent = 0);

    void emitlog(QString s, logger::LogLevel level, QObject* pLogTarget);
signals:
    void log(QString s, logger::LogLevel level);

public slots:
};

#endif // EMITTER_H
