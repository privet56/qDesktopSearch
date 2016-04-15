#include "wsearch.h"
#include "str.h"
#include "searcher.h"
#include "ui_wsearch.h"
#include "wsearchresultmodel.h"

wSearch::wSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wSearch)
{
    ui->setupUi(this);

    {
        wsearchresultModel* pWsearchresultModel = new wsearchresultModel(new searcher(), this->ui->tableView);
        this->ui->tableView->setModel(pWsearchresultModel);
    }

    on_eSearchTerm_textChanged("");
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

}

void wSearch::on_tableView_clicked(const QModelIndex &index)
{

}

void wSearch::on_tableView_doubleClicked(const QModelIndex &index)
{

}

void wSearch::on_tableView_customContextMenuRequested(const QPoint &pos)
{

}
