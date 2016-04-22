#include "wsearch.h"
#include "str.h"
#include "lucy.h"
#include "searcher.h"
#include "ui_wsearch.h"
#include "wsearchresultmodel.h"
#include <QPair>
#include <QTimer>
#include <QList>
#include "wsearchitemdelegate.h"

wSearch::wSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wSearch),
    m_pIndexer(nullptr),
    m_pLog(nullptr),
    m_pSearcher(new searcher())
{
    ui->setupUi(this);

    {
        wsearchresultModel* pWsearchresultModel = new wsearchresultModel(m_pSearcher, this->ui->tableView);
        this->ui->tableView->setModel(pWsearchresultModel);
        this->ui->tableView->setItemDelegate(new wsearchitemdelegate(this->ui->tableView));
    }

    on_eSearchTerm_textChanged("");

    QTimer::singleShot(4999/*msec = 5 secs*/, this, SLOT(fillFields()));

    //TODO: fill(+update later) field dropdown(with multiple selection?)
}

wSearch::~wSearch()
{
    delete ui;
}

void wSearch::on_eSearchTerm_textChanged(const QString &arg1)
{
    this->ui->commandLinkButtonSearch->setEnabled(!str::isempty(arg1));
}

void wSearch::on_commandLinkButtonSearch_clicked()
{
    QTime t;
    t.start();
    QPair<QString, QString> pair;
    pair.first  = this->ui->eSearchTerm->text();
    pair.second = QString::fromStdWString(FIELDNAME_FULLTEXT);
    QList<QPair<QString, QString>> lpSearchInputs;
    lpSearchInputs.append(pair);

    wsearchresultModel* pModel = (wsearchresultModel*)this->ui->tableView->model();
    int hits = (pModel)->search(lpSearchInputs);
    wsearchresultModel* pModel2 = new wsearchresultModel(nullptr, nullptr);
    //TODO: do it better (emit dataChanged?)
    this->ui->tableView->setModel(pModel2);
    this->ui->tableView->setModel(pModel);
    delete pModel2;
    pModel2 = nullptr;
    this->ui->lHitCount->setText(QString::number(hits)+" hits found in "+logger::t_elapsed(t.elapsed()));
}

void wSearch::on_tableView_clicked(const QModelIndex &index)
{

}

void wSearch::on_tableView_doubleClicked(const QModelIndex &index)
{
    //TODO: open selected
}

void wSearch::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    //TODO: show all details
}

void wSearch::setEnv(logger* pLog, indexer* pIndexer)
{
    this->m_pLog        = pLog;
    this->m_pIndexer    = pIndexer;
    ((wsearchresultModel*)this->ui->tableView->model())->setEnv(pLog, pIndexer);

    connect(pLog->GetCfg(), SIGNAL(cfgChanged(QString, QString)), this, SLOT(OnCfgChanged(QString, QString)));
}
void wSearch::OnCfgChanged(QString sCfgEntryName, QString sCfgEntryValue)
{
    if(sCfgEntryName != DIRS2INDEX)
    {
        return;
    }

    wsearchresultModel* pModel = (wsearchresultModel*)this->ui->tableView->model();
    pModel->reset();
    wsearchresultModel* pModel2 = new wsearchresultModel(nullptr, nullptr);
    //TODO: do it better (emit dataChanged?)
    this->ui->tableView->setModel(pModel2);
    this->ui->tableView->setModel(pModel);
    delete pModel2;
    pModel2 = nullptr;
    this->ui->lHitCount->setText("-");

    this->m_pLog->inf("result list resetted because of indexer changes");
}

void wSearch::fillFields()
{
    QSet<QString> slFields = m_pSearcher->fields();
    QList<QString> list = slFields.toList();
    qSort(list);
    //TODO: fill cb!
}
