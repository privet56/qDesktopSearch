#include <QStringList>
#include <QFileDialog>
#include "wsettings.h"
#include "ui_wsettings.h"
#include "wsettingdirs2indexmodel.h"

wSettings::wSettings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wSettings),
    m_pIndexer(nullptr),
    m_pLog(nullptr)
{
    ui->setupUi(this);
    ui->frameSave->setVisible(false);
    this->ui->toolButtonDel->setEnabled(false);
}

wSettings::~wSettings()
{
    delete ui;
}

void wSettings::on_toolButtonAdd_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(dialog.exec() != QDialog::Accepted)return;
    QString sDir2Index = str::normalizePath(dialog.selectedFiles()[0], false);

    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);

    if(sl.contains(sDir2Index))return;
    sl.append(sDir2Index);
    this->m_pLog->GetCfg()->setValue(DIRS2INDEX, sl.join('|'));

    refreshtableViewDirs(sl.size());

    m_pIndexer->add(sDir2Index);
}

void wSettings::on_toolButtonDel_clicked()
{
    QModelIndexList sels = this->ui->tableViewDirs->selectionModel()->selectedIndexes();
    if(sels.size() != 1)
    {
         this->m_pLog->wrn("what is selected("+QString::number(sels.size())+")?");
         return;
    }
    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
    int i2remove = sels.at(0).row();
    QString s2remove = sl.at(i2remove);
    sl.removeAt(i2remove);

    this->m_pLog->GetCfg()->setValue(DIRS2INDEX, sl.join('|'));
    refreshtableViewDirs(sl.size());
    m_pIndexer->removeIndex(s2remove);
}

void wSettings::on_tableViewDirs_clicked(const QModelIndex &index)
{
    QModelIndexList sels = this->ui->tableViewDirs->selectionModel()->selectedIndexes();
    this->ui->toolButtonDel->setEnabled(index.isValid() && (sels.size() == 1));
}
void wSettings::setEnv(logger* pLog, indexer* pIndexer)
{
    this->m_pLog        = pLog;
    this->m_pIndexer    = pIndexer;
    wsettingdirs2indexModel* pModel = new wsettingdirs2indexModel(this->ui->tableViewDirs);
    pModel->setEnv(pLog);
    this->ui->tableViewDirs->setModel(pModel);
}
void wSettings::refreshtableViewDirs(int iCfgEntriesLen)
{
    //TODO: clear search result list
    {

    }

    wsettingdirs2indexModel* pModel = (wsettingdirs2indexModel*)this->ui->tableViewDirs->model();
    wsettingdirs2indexModel* pModel2 = new wsettingdirs2indexModel(nullptr);
    //TODO: do it better (emit dataChanged?)
    this->ui->tableViewDirs->setModel(pModel2);
    this->ui->tableViewDirs->setModel(pModel);
    delete pModel2;
    pModel2 = nullptr;

    {   //handle selection
        this->ui->toolButtonDel->setEnabled(false);

        if(iCfgEntriesLen > 0)
        {
            this->ui->tableViewDirs->selectRow(1);
        }
    }
}
