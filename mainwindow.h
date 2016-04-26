#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplashScreen>
#include "cfg.h"
#include "logger.h"
#include "indexer.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSplashScreen* pSplash, logger* pLog, indexer* pIndexer, QWidget *parent = 0);
    ~MainWindow();

    logger* m_pLog;
    indexer* m_pIndexer;

private slots:
    void on_actionExit_triggered();

    void on_actionMinimize_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
