#ifndef STR_H
#define STR_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QDirIterator>
#include <QMap>
#include <QStringList>

class str : public QObject
{
    Q_OBJECT
public:
    explicit str(QObject *parent = 0);

    static QString mult(QString s, int iMultiplicator);
    static QString makeAbsFN(QString sBase, QString sAppend);
    static QString normalizePath(QString sPath, bool bLowerCase);
    static bool isempty(QString s, bool bTrim=true);
    static QString intWithThousandSeps(int i);

signals:

public slots:
};

#endif // STR_H
