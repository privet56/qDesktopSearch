#ifndef WFIELDSMODEL_H
#define WFIELDSMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "globalinclude.h"
#include "logger.h"
#include "cfg.h"
#include "searcher.h"

class wFieldsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit wFieldsModel(logger* pLogger, searcher* pSearcher, QObject *parent = 0);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;


    void setEnv(logger* pLogger, searcher* pSearcher);
    void setFields(QStringList sl);

protected:
    QStringList m_slFields;
    logger* m_pLogger;
    searcher* m_pSearcher;

signals:

public slots:
};

#endif // WFIELDSMODEL_H
