#include "wsearchresultmodel.h"
#include "globalinclude.h"
#include <QBrush>
#include <QIcon>
#include <QFileIconProvider>

/*!
 * Creates a model object with the given \a ds and \a parent.
 */
wsearchresultModel::wsearchresultModel(searcher* pSearcher, QObject *parent)
    :QAbstractTableModel(parent), m_pSearcher(pSearcher),
      m_pIndexer(nullptr),
      m_pLog(nullptr)
{

}

void wsearchresultModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder*/)
{
    if(!this->m_pSearcher)
        return;

    if(column == COL_FILENAME)
        this->m_pSearcher->sortBy(QString::fromStdWString(FIELDNAME_ABSPATHNAME), order);
    else if(column == COL_HITENV)
        this->m_pLog->inf("not sortable by hitenv");
    else
        this->m_pLog->wrn("unknown col to be sorted");

    emit layoutChanged();
}

/*!
 * Destroys the model.
 */
wsearchresultModel::~wsearchresultModel()
{

}

int wsearchresultModel::rowCount(const QModelIndex &/*parent*/) const
{
    int iHitCount = m_pSearcher == nullptr ? 0 :  m_pSearcher->getHitCount();
    return iHitCount;
}

int wsearchresultModel::columnCount(const QModelIndex &/*parent*/) const
{
    //1:fn(tooltip: fullpath(or all infos)), 2:hitEnv
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
    if (!index.isValid() || !m_pSearcher)
        return QVariant();

    int hitIndex2BeReturned = index.row();
    int column2BeReturned   = index.column();

    if((role == Qt::DisplayRole) || (role == Qt::ToolTipRole) || (role == Qt::StatusTipRole))
    {
        if(column2BeReturned == COL_FILENAME)
        {
            if(role == Qt::ToolTipRole)
            {
                return m_pSearcher->GetHitAttr(hitIndex2BeReturned, QString::fromStdWString(FIELDNAME_ABSPATHNAME));
            }
            return m_pSearcher->GetHitAttr(hitIndex2BeReturned, QString::fromStdWString(FIELDNAME_FILENAME));
        }
        if(column2BeReturned == COL_HITENV)
        {
            return m_pSearcher->GetHitEnv(hitIndex2BeReturned);
        }

        this->m_pLog->wrn("unknown role to be returned...");
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
            case COL_FILENAME:
                align |= Qt::AlignJustify;
            default:
				align |= Qt::AlignLeft;//Qt::AlignRight,Qt::AlignHCenter,Qt::AlignJustify
                break;
        }
        switch (column2BeReturned)
        {
            case COL_FILENAME:
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
        if (orientation == Qt::Vertical)
        {
            if(m_pSearcher)
            {
                QFileInfo fi(m_pSearcher->GetHitAttr(section, QString::fromStdWString(FIELDNAME_ABSPATHNAME)));
                static QFileIconProvider fip;
                return fip.icon(fi);
            }
            return QIcon(":/res/hit.png");
        }
        if (orientation == Qt::Horizontal)
        {

        }
    }
    else if(role == Qt::SizeHintRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if(section == 0)
                return QVariant(QSize(20, 20));
            if(section == 1)//TODO: why is this not working?
                return QVariant(QSize(500, 20));
        }
    }
    else if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
			//column headers
            if(section == COL_FILENAME)  return "file name";
            if(section == COL_HITENV)    return "hit environment";
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
void wsearchresultModel::setEnv(logger* pLog, indexer* pIndexer)
{
    this->m_pLog        = pLog;
    this->m_pIndexer    = pIndexer;
    if (this->m_pSearcher)
        this->m_pSearcher->setEnv(pLog, pIndexer);
}
int wsearchresultModel::search(QList<QPair<QString, QString>> lpSearchinputs)
{
    int iHits = this->m_pSearcher->search(lpSearchinputs);
    return iHits;
}
QModelIndex wsearchresultModel::buddy(const QModelIndex &index) const
{
    return index;
}
void wsearchresultModel::reset()
{
    if (this->m_pSearcher)
        this->m_pSearcher->cleanup(false);
}
