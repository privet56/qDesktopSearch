#include "wsearch.h"
#include "str.h"
#include "lucy.h"
#include "searcher.h"
#include "ui_wsearch.h"
#include "wsearchresultmodel.h"
#include <QPair>
#include <QMenu>
#include <QTimer>
#include <QTableWidget>
#include <QList>
#include <QDesktopServices>
#include "wsearchitemdelegate.h"
#include "wfieldsmodel.h"

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

    this->ui->cbField->setModel(new wFieldsModel(this->m_pLog, this->m_pSearcher));
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
    pair.second = this->ui->cbField->currentText();//QString::fromStdWString(FIELDNAME_FULLTEXT);
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

void wSearch::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_actionOpen();
}

void wSearch::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = qobject_cast<QWidget*>(sender())->mapToGlobal(pos);
    globalPos.setY(globalPos.y() + 25);

    QMenu menu;
    QIcon iInfo(":/res/information.png");
    QIcon iOpen(":/res/quickopen.png");
    menu.addAction(iInfo, "Info..." , this, SLOT(on_actionInfo()));
    menu.addAction(iOpen, "Open"    , this, SLOT(on_actionOpen()));
    menu.exec(globalPos);
}

void wSearch::setEnv(logger* pLog, indexer* pIndexer)
{
    this->m_pLog        = pLog;
    this->m_pIndexer    = pIndexer;

    ((wsearchresultModel*)this->ui->tableView->model())->setEnv(pLog, pIndexer);
    ((wFieldsModel*)this->ui->cbField->model())->setEnv(this->m_pLog, this->m_pSearcher);

    connect(pLog->GetCfg(), SIGNAL(cfgChanged(QString, QString)), this, SLOT(OnCfgChanged(QString, QString)));
    QTimer::singleShot(4999/*msec = 5 secs*/, this, SLOT(fillFields()));
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
    if(!m_pSearcher)return;

    QSet<QString> slFields = m_pSearcher->fields();
    QList<QString> list = slFields.toList();
    qSort(list);
    ((wFieldsModel*)this->ui->cbField->model())->setFields(list);
    this->ui->cbField->setCurrentText("text");

    QTimer::singleShot(4999 * 10/*msec = 5*10 secs*/, this, SLOT(fillFields()));
}
void wSearch::on_actionInfo()
{
    QModelIndexList sels = this->ui->tableView->selectionModel()->selectedRows();
    if(sels.size() != 1)return;
    int iHit = sels.at(0).row();
    QMap<QString, QString> mallAttrs = m_pSearcher->GetHitAttrs(iHit);
    QTableWidget* pAllAttrsTable = new QTableWidget(nullptr);
    pAllAttrsTable->setAttribute(Qt::WA_DeleteOnClose);
    pAllAttrsTable->setColumnCount(2);
    pAllAttrsTable->setAlternatingRowColors(true);
    pAllAttrsTable->setAutoFillBackground(true);
    pAllAttrsTable->setColumnWidth(0, 199);
    pAllAttrsTable->setCornerButtonEnabled(true);
    pAllAttrsTable->setShowGrid(true);
    pAllAttrsTable->setSortingEnabled(true);
    pAllAttrsTable->horizontalHeader()->setStretchLastSection(true);
    QStringList slTableHeader;
    slTableHeader << "Attribute" << "Value";
    pAllAttrsTable->setHorizontalHeaderLabels(slTableHeader);

    QMapIterator<QString, QString> i(mallAttrs);

    int iRow = -1;
    while(i.hasNext())
    {
        iRow++;
        i.next();

        QTableWidgetItem *i0 = new QTableWidgetItem(i.key());
        QTableWidgetItem *i1 = new QTableWidgetItem(i.value());

        pAllAttrsTable->insertRow(iRow);
        pAllAttrsTable->setItem(iRow,0,i0);
        pAllAttrsTable->setItem(iRow,1,i1);
    }
    pAllAttrsTable->setMinimumSize(QSize(555,355));
    pAllAttrsTable->show();
}

void wSearch::on_actionOpen()
{
    QModelIndexList sels = this->ui->tableView->selectionModel()->selectedRows();
    if(sels.size() != 1)
    {
        if(this->m_pLog)this->m_pLog->inf("no hit selected "+QString::number(sels.size()));
        return;
    }
    int iHit = sels.at(0).row();
    QString sAbsFN = m_pSearcher->GetHitAttr(iHit, QString::fromStdWString(FIELDNAME_ABSPATHNAME));
    if(str::isempty(sAbsFN, true))
    {
        if(this->m_pLog)this->m_pLog->wrn("filename for hit nr. "+QString::number(iHit)+" not found");
        return;
    }
    if(!QDesktopServices::openUrl(QUrl(sAbsFN)))
    {
        if(this->m_pLog)this->m_pLog->wrn("failed to execute '"+sAbsFN+"'");
    }
}
