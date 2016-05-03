#ifndef F_H
#define F_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QDirIterator>
#include <QMap>
#include <QStringList>
#include "logger.h"

class f : public QObject
{
    Q_OBJECT
public:
    explicit f(QObject *parent = 0);

    static bool emptydir(QString sDir, logger* pLog);
    static QString fileSizeAsString(qint64 size);

    static void emptydir(QString sDir, QString sPattern, bool bRecursive, int& iDeleteds, int& iDeletionFaileds);
    static QString getFC(QString sAbsFN, logger* pLog = nullptr);

signals:

public slots:

};

#endif // F_H
