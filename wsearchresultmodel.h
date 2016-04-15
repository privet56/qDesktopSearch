#ifndef WSEARCHRESULTMODEL_H
#define WSEARCHRESULTMODEL_H

#include <QAbstractTableModel>
#include "searcher.h"

class wsearchresultModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit wsearchresultModel(searcher* pSearcher, QObject *parent = 0);
    ~wsearchresultModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

protected:
    searcher* m_pSearcher;

signals:

public slots:

};

#endif // WSEARCHRESULTMODEL_H
