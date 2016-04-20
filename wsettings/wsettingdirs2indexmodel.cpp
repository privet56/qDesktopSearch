#include "wsettingdirs2indexmodel.h"

#include <QBrush>
#include <QIcon>
#include "globalinclude.h"
#include "str.h"

/*!
 * Creates a model object with the given \a ds and \a parent.
 */
wsettingdirs2indexModel::wsettingdirs2indexModel(QObject *parent)
    :QAbstractTableModel(parent),
      m_pLog(nullptr)
{

}

/*!
 * Destroys the model.
 */
wsettingdirs2indexModel::~wsettingdirs2indexModel()
{

}

int wsettingdirs2indexModel::rowCount(const QModelIndex &/*parent*/) const
{
    if(!this->m_pLog)return 0;

    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);

    return sl.size();
}

int wsettingdirs2indexModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

Qt::ItemFlags wsettingdirs2indexModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;// | Qt::ItemIsEditable;
}

QVariant wsettingdirs2indexModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int iRow = index.row();
    int iCol = index.column();

    if((role == Qt::DisplayRole) || (role == Qt::ToolTipRole) || (role == Qt::StatusTipRole))
    {
        QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
        QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
        return sl.at(iRow);
    }
    else if (role == Qt::EditRole)
    {
        return QVariant();	//should not happen
    }
    else if (role == Qt::TextAlignmentRole)
    {
        Qt::Alignment align;
        switch (iCol)
        {
            case 0:
                align |= Qt::AlignJustify;
            default:
				align |= Qt::AlignLeft;//Qt::AlignRight,Qt::AlignHCenter,Qt::AlignJustify
                break;
        }
        switch (iCol)
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
		return QVariant();
    }
    else if (role == Qt::BackgroundRole)
    {
		return QVariant();
    }

    return QVariant();
}

QVariant wsettingdirs2indexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DecorationRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return QIcon(":/res/indexedDB.png");
        }
    }
    else if(role == Qt::SizeHintRole)
    {
	
    }
    else if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
			//column headers
			if(section == 0)return "Directory to be indexed";
        }
        else
        {
			//row 'header'
            return QString::number(section + 1);
        }
    }
    return QVariant();
}

bool wsettingdirs2indexModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
void wsettingdirs2indexModel::setEnv(logger* pLog)
{
    this->m_pLog        = pLog;
}

QModelIndex wsettingdirs2indexModel::buddy(const QModelIndex &index) const
{
    return index;
}
