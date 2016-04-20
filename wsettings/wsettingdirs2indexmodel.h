#ifndef SETTINGDIRS2INDEX_H
#define SETTINGDIRS2INDEX_H

#include <QAbstractTableModel>
#include "logger.h"

class wsettingdirs2indexModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit wsettingdirs2indexModel(QObject *parent = 0);
    ~wsettingdirs2indexModel();

    void setEnv(logger* pLog);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex buddy(const QModelIndex &index) const;

protected:
    logger* m_pLog;

signals:

public slots:

};

#endif // SETTINGDIRS2INDEX_H
