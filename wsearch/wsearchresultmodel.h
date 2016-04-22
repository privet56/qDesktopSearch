#ifndef WSEARCHRESULTMODEL_H
#define WSEARCHRESULTMODEL_H

#include <QAbstractTableModel>
#include "searcher.h"
#include "logger.h"
#include "indexer.h"



class wsearchresultModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit wsearchresultModel(searcher* pSearcher, QObject *parent = 0);
    ~wsearchresultModel();

    void setEnv(logger* pLog, indexer* pIndexer);
    int search(QList<QPair<QString, QString>> lpSearchinputs);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex buddy(const QModelIndex &index) const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void reset();
    searcher* getSearcher();

    static const int COL_FILENAME  = 0;
    static const int COL_HITENV    = 1;

protected:
    searcher* m_pSearcher;

protected:
    logger* m_pLog;
    indexer* m_pIndexer;

signals:

public slots:

};

#endif // WSEARCHRESULTMODEL_H
