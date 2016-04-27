#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include "wsettings.h"
#include "ui_wsettings.h"
#include "wsettingdirs2indexmodel.h"
#include "balloon.h"
#include "tray.h"
#include "idxinfo.h"
#include "f.h"
#include <QMetaObject>
#include <QTimer>
#include <QMenu>
#include <QDesktopServices>

wSettings::wSettings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wSettings),
    m_pIndexer(nullptr),
    m_pLog(nullptr)
{
    ui->setupUi(this);
    ui->frameSave->setVisible(false);
    this->ui->toolButtonDel->setEnabled(false);
    this->ui->toolButtonInfo->setEnabled(false);

    QTimer::singleShot(1999/*msec = 2 secs*/, this, SLOT(on_init()));
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
    this->ui->toolButtonInfo->setEnabled(index.isValid() && (sels.size() == 1));

    QModelIndex sel = sels.at(0);
/*
    QItemSelection rowSelection;

    QModelIndex topLeft = this->ui->tableViewDirs->model()->index(0, 0, QModelIndex());
    QModelIndex bottomRight = this->ui->tableViewDirs->model()->index(1, 0, QModelIndex());
    rowSelection.select(topLeft, bottomRight);
*/
    this->ui->tableWidgetDirsInfo->selectionModel()->clear();
    this->ui->tableWidgetDirsInfo->selectionModel()->select(sel, QItemSelectionModel::Select | QItemSelectionModel::Rows);
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
    wsettingdirs2indexModel* pModel = (wsettingdirs2indexModel*)this->ui->tableViewDirs->model();
    wsettingdirs2indexModel* pModel2 = new wsettingdirs2indexModel(nullptr);
    //TODO: do it better (emit dataChanged?)
    this->ui->tableViewDirs->setModel(pModel2);
    this->ui->tableViewDirs->setModel(pModel);
    delete pModel2;
    pModel2 = nullptr;

    {   //handle selection
        this->ui->toolButtonDel->setEnabled(false);
        this->ui->toolButtonInfo->setEnabled(false);

        if(iCfgEntriesLen > 0)
        {
            this->ui->tableViewDirs->selectRow(1);
        }
    }
}

void wSettings::on_updateIdxInfo()
{
    QTableWidget* pAllAttrsTable = this->ui->tableWidgetDirsInfo;
    static bool bUISet = false;

    if(!bUISet)
    {   //config table
        bUISet = true;
        pAllAttrsTable->setColumnCount(2);
        pAllAttrsTable->setAlternatingRowColors(true);
        pAllAttrsTable->setAutoFillBackground(true);
        pAllAttrsTable->setColumnWidth(0, 77);
        pAllAttrsTable->setCornerButtonEnabled(true);
        pAllAttrsTable->setShowGrid(true);
        pAllAttrsTable->setSortingEnabled(false);
        pAllAttrsTable->horizontalHeader()->setStretchLastSection(true);
        QStringList slTableHeader;
        slTableHeader << "Idx Size" << "# Idx Files";
        pAllAttrsTable->setHorizontalHeaderLabels(slTableHeader);
    }

    pAllAttrsTable->setRowCount(0);

    if(this->m_pIndexer)
    {
        QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
        QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
        for(int i=0;i<sl.size();i++)
        {
            IdxInfo idxi;
            this->m_pIndexer->getIdxInfo(&idxi, sl.at(i), false/*askLucy*/);
            QTableWidgetItem *i1 = new QTableWidgetItem(f::fileSizeAsString((qint64)idxi.indexDirSize()));
            QTableWidgetItem *i2 = new QTableWidgetItem(QString::number(idxi.indexDirNrOfFiles()));
            {
                QString sTT = "dir to index:   " +idxi.dir2Index()                       +"\n"+
                              "dir of idx:   " +idxi.indexDir()                        +"\n"+
                              "index size:   " +f::fileSizeAsString(idxi.indexDirSize())+"\n"+
                              "number of index files:   " +QString::number(idxi.indexDirNrOfFiles());
                i1->setToolTip(sTT);
                i2->setToolTip(sTT);
            }

            pAllAttrsTable->insertRow(i);
            pAllAttrsTable->setItem(i,0,i1);
            pAllAttrsTable->setItem(i,1,i2);
        }
    }

    QTimer::singleShot(29999/*msec = 30 secs*/, this, SLOT(on_updateIdxInfo()));
}

void wSettings::on_toolButtonInfo_clicked()
{
    if(!this->m_pIndexer)
    {
        return;
    }

    QModelIndexList sels = this->ui->tableViewDirs->selectionModel()->selectedIndexes();
    if(sels.size() != 1)
    {
         this->m_pLog->wrn("what is selected("+QString::number(sels.size())+")?");
         return;
    }

    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
    int i2Info = sels.at(0).row();
    QString sDir2Index2Info(sl.at(i2Info));

    QTableWidget* pAllAttrsTable = new QTableWidget(nullptr);
    pAllAttrsTable->setAttribute(Qt::WA_DeleteOnClose);

    {   //config table
        pAllAttrsTable->setColumnCount(2);
        pAllAttrsTable->setAlternatingRowColors(true);
        pAllAttrsTable->setAutoFillBackground(true);
        pAllAttrsTable->setColumnWidth(0, 190);
        pAllAttrsTable->setCornerButtonEnabled(true);
        pAllAttrsTable->setShowGrid(true);
        pAllAttrsTable->setSortingEnabled(true);
        pAllAttrsTable->horizontalHeader()->setStretchLastSection(true);
        QStringList slTableHeader;
        slTableHeader << "Attribute" << "Value";
        pAllAttrsTable->setHorizontalHeaderLabels(slTableHeader);
    }
    {
        IdxInfo idxi;
        this->m_pIndexer->getIdxInfo(&idxi, sDir2Index2Info, true/*askLucy*/);

        /*//TODO: do it dynamic
        QMetaObject *meta = idxi.metaObject();
        int n = meta->propertyCount();
        for(int i = 0; i < n; ++i) {
            QMetaProperty p = meta->property(i);    //prop with name, type, value
        }*/

        QString sTT = "dir to index:   " +idxi.dir2Index()                         +"\n"+
                      "dir of idx:   " +idxi.indexDir()                            +"\n"+
                      "index size:   " +f::fileSizeAsString(idxi.indexDirSize())   +"\n"+
                      "number of index files:   " +QString::number(idxi.indexDirNrOfFiles());

        int iRow = 0;
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Directory to be indexed");
            QTableWidgetItem *i1 = new QTableWidgetItem(idxi.dir2Index());
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Directory if the Index");
            QTableWidgetItem *i1 = new QTableWidgetItem(idxi.indexDir());
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Size of the Index");
            QTableWidgetItem *i1 = new QTableWidgetItem(f::fileSizeAsString((qint64)idxi.indexDirSize()));
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Nr of Files of the Index");
            QTableWidgetItem *i1 = new QTableWidgetItem(QString::number(idxi.indexDirNrOfFiles()));
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Nr of Files in the Index (Indexed Files)");
            QTableWidgetItem *i1 = new QTableWidgetItem(str::intWithThousandSeps((int)idxi.indexNrOfFilesInIndex()));
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Indexed in the current Index Loop");
            QTableWidgetItem *i1 = new QTableWidgetItem(QString::number(idxi.indexNrOfIndexedFilesInCurrentLoop()));
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
        {
            QTableWidgetItem *i0 = new QTableWidgetItem("Index is New");
            QTableWidgetItem *i1 = new QTableWidgetItem(idxi.indexIsNew() ? "true" : "false");
            i0->setToolTip(sTT);
            i1->setToolTip(sTT);
            pAllAttrsTable->insertRow(iRow);
            pAllAttrsTable->setItem(iRow,0,i0);
            pAllAttrsTable->setItem(iRow,1,i1);
            iRow++;
        }
    }

    pAllAttrsTable->setMinimumSize(QSize(955,355));
    pAllAttrsTable->show();
}

void wSettings::on_init()
{
    int iDirCount = this->ui->tableViewDirs->model()->rowCount();
    if (iDirCount < 1)
        tray::getTrayIcon()->showMessage("Hint", "You have no directories in the Settings pane configured to be indexed.    ", QSystemTrayIcon::Information, 1000);
    else
        tray::getTrayIcon()->showMessage("Hint", "Starting indexing "+QString::number(iDirCount)+" director"+((iDirCount > 1) ? "ies" : "y")+"...           ", QSystemTrayIcon::Information, 1000);

    QTimer::singleShot(4999/*msec = 5 secs*/, this, SLOT(on_updateIdxInfo()));
}

void wSettings::on_tableViewDirs_customContextMenuRequested(const QPoint &pos)
{
    QModelIndexList sels = this->ui->tableViewDirs->selectionModel()->selectedIndexes();
    if(sels.size() != 1)
    {
         this->m_pLog->wrn("what is selected("+QString::number(sels.size())+")?");
         return;
    }

    this->ui->tableWidgetDirsInfo->selectionModel()->clear();
    this->ui->tableWidgetDirsInfo->selectionModel()->select(sels.at(0), QItemSelectionModel::Select | QItemSelectionModel::Rows);

    contextMenuAt(pos, sels.at(0).row());
}

void wSettings::on_tableWidgetDirsInfo_customContextMenuRequested(const QPoint &pos)
{
    QModelIndexList sels = this->ui->tableWidgetDirsInfo->selectionModel()->selectedRows();

    if(sels.size() != 1)
    {
         this->m_pLog->wrn("what is selected in info tbl ("+QString::number(sels.size())+")?");
         return;
    }

    this->ui->tableViewDirs->selectionModel()->clear();
    this->ui->tableViewDirs->selectionModel()->select(sels.at(0), QItemSelectionModel::Select | QItemSelectionModel::Rows);

    contextMenuAt(pos, sels.at(0).row());
}
void wSettings::contextMenuAt(QPoint pos, int selectedItem)
{
    Q_UNUSED(selectedItem)

    QPoint globalPos = qobject_cast<QWidget*>(sender())->mapToGlobal(pos);
    globalPos.setY(globalPos.y() + 25);
    globalPos.setX(globalPos.x() + 25);

    QMenu menu;
    QIcon iInfo(":/res/information.png");
    QIcon iOpen(":/res/quickopen.png");
    menu.addAction(iInfo, "Info..." , this, SLOT(on_toolButtonInfo_clicked()));
    menu.addAction(iOpen, "Open"    , this, SLOT(on_actionOpen()));
    menu.exec(globalPos);
}

void wSettings::on_actionOpen()
{
    QModelIndexList sels = this->ui->tableViewDirs->selectionModel()->selectedIndexes();
    if(sels.size() != 1)
    {
         this->m_pLog->wrn("what is selected("+QString::number(sels.size())+")?");
         return;
    }

    QString sDirs2Index(this->m_pLog->GetCfg()->getValue(DIRS2INDEX));
    QStringList sl = sDirs2Index.split('|', QString::SkipEmptyParts);
    int i2Info = sels.at(0).row();
    QString sDir2Index2Open(sl.at(i2Info));

    //TODO: open also unc paths correctly
    //sDir2Index2Open = sDir2Index2Open.replace("/", "\\\\");

    if(!QDesktopServices::openUrl(sDir2Index2Open))
    {
        this->m_pLog->wrn("could not open "+sDir2Index2Open);
    }
}
