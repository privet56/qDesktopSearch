#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QPair>
#include <QVector>
#include <QList>
#include <QSet>
#include "logger.h"
#include "seacherHighlightFormatter.h"
#include "indexer.h"
#include "lucysearcher.h"

#define MAX_SEARCHERS 99

class searcher : public QObject
{
    Q_OBJECT
public:
    explicit searcher(QObject *parent = 0);

    void setEnv(logger* pLog, indexer* pIndexer);

    int search(QList<QPair<QString, QString>> lpSearchinputs);

    int getHitCount();
    QString GetHitAttr(int iHitNr, QString sAttrName);
    QMap<QString, QString> GetHitAttrs(int iHitNr);
    QString GetHitEnv(int iHitNr);
    void cleanup(bool bConstructor);
    void sortBy(QString sSortFieldName, Qt::SortOrder order);
    QSet<QString> fields();

protected:
    logger* m_pLog;
    indexer* m_pIndexer;

    MultiSearcher* m_pMultiSearcher;

    Searchable** m_searchables;
    QList<lucysearcher*> m_lucysearchables;
    ArrayBase<Query*>* m_aquerys;
    BooleanQuery* m_query;
    Hits* m_hits;
    TopFieldDocs* m_tophits;
    Sort* m_sort;

signals:

public slots:
};

#endif // SEARCHER_H
