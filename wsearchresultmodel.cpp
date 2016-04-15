#include "wsearchresultmodel.h"

#include <QBrush>
#include <QIcon>

/*!
 * Creates a model object with the given \a ds and \a parent.
 */
wsearchresultModel::wsearchresultModel(searcher* pSearcher, QObject *parent)
    :QAbstractTableModel(parent), m_pSearcher(pSearcher)
{

}

/*!
 * Destroys the model.
 */
wsearchresultModel::~wsearchresultModel()
{

}

int wsearchresultModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_pSearcher->getHitCount();
}

int wsearchresultModel::columnCount(const QModelIndex &/*parent*/) const
{
//1:fn(tooltip: fullpath), 2:hitEnvironment
    return 2;
}

Qt::ItemFlags wsearchresultModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;// | Qt::ItemIsEditable;
}

QVariant wsearchresultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int hitIndex2BeReturned = index.row();
    int column2BeReturned   = index.column();

    if((role == Qt::DisplayRole) || (role == Qt::ToolTipRole) || (role == Qt::StatusTipRole))
    {
		if(column2BeReturned == 0)
            return m_pSearcher->GetHitAttr(hitIndex2BeReturned, "filename");
		if(column2BeReturned == 1)
            return m_pSearcher->GetHitEnv(hitIndex2BeReturned);
		//TODO: wrn
    }
    else if (role == Qt::EditRole)
    {
        return QVariant();	//should not happen
    }
    else if (role == Qt::TextAlignmentRole)
    {
        Qt::Alignment align;
        switch (column2BeReturned)
        {
            case 0:
                align |= Qt::AlignJustify;
            default:
				align |= Qt::AlignLeft;//Qt::AlignRight,Qt::AlignHCenter,Qt::AlignJustify
                break;
        }
        switch (column2BeReturned)
        {
            case 0:
                align |= Qt::AlignVCenter;
            default:
				align |= Qt::AlignVCenter;//Qt::AlignRight,Qt::AlignHCenter,Qt::AlignJustify
                break;
        }
        return QVariant(align);
    }
    else if (role == Qt::FontRole)
    {
		return QVariant();
    }
    else if (role == Qt::ForegroundRole)
    {
		return QVariant();	//return QBrush("fontColor");
    }
    else if (role == Qt::BackgroundRole)
    {
		return QVariant();	//return QBrush("fontColor");
    }

    return QVariant();
}

QVariant wsearchresultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DecorationRole)
    {
		//TODO: get file icon
        if (orientation == Qt::Vertical)
            return QIcon(":/res/hit.png");
    }
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
			//column headers
			if(section == 0)return "file name";
			if(section == 1)return "hit environment";
        }
        else
        {
			//row 'header'
            return QString::number(section + 1);
        }
    }
    return QVariant();
}

bool wsearchresultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(value)

    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
    {	//should not happen
		return false;
    }

    return false;
}
