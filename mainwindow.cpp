#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QSplashScreen* pSplash, logger* pLog, indexer* pIndexer, QWidget *parent) :
    QMainWindow(parent),
    m_pLog(pLog),
    m_pIndexer(pIndexer),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->setEnv(this->m_pLog, this->m_pIndexer);
    ui->scrollAreaWidgetContents_2->setEnv(this->m_pLog, this->m_pIndexer);

    this->ui->eLog->SetStatusBar(this->statusBar());
    this->m_pLog->SetLogWnd(this->ui->eLog);

    QTimer::singleShot(999, pSplash, SLOT(close()));
    this->m_pLog->inf("App Startup Finished");

    m_pIndexer->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
