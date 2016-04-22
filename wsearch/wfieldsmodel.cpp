#include "wfieldsmodel.h"

wFieldsModel::wFieldsModel(logger* pLogger, searcher* pSearcher, QObject *parent) : QAbstractTableModel(parent), m_pLogger(pLogger), m_pSearcher(pSearcher)
{

}

int wFieldsModel::columnCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    return 1;//QAbstractItemModel::columnCount(parent);
}

QVariant wFieldsModel::data(const QModelIndex &index, int role/* = Qt::DisplayRole*/) const
{
    if((role == Qt::DisplayRole) || (role == Qt::ToolTipRole) || (role == Qt::StatusTipRole))
    {
        return m_slFields.at(index.row());
    }
    return QVariant();//QAbstractItemModel::data(index, role);
}

QModelIndex wFieldsModel::index(int row, int column, const QModelIndex &parent/* = QModelIndex()*/) const
{
    return this->createIndex(row, column);
}

QVariant wFieldsModel::headerData(int section, Qt::Orientation orientation, int role/* = Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            //column headers
            return "Fields";
        }
    }

    QAbstractItemModel::headerData(section, orientation);
}

int wFieldsModel::rowCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    return this->m_slFields.length();//QAbstractItemModel::rowCount(parent);
}
void wFieldsModel::setEnv(logger* pLogger, searcher* pSearcher)
{
    this->m_pLogger     = pLogger;
    this->m_pSearcher   = pSearcher;
}
void wFieldsModel::setFields(QStringList sl)
{
    m_slFields.clear();
    m_slFields << sl;

    emit layoutChanged();
}
