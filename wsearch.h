#ifndef WSEARCH_H
#define WSEARCH_H

#include <QFrame>
#include "searcher.h"
#include "logger.h"
#include "indexer.h"

namespace Ui {
class wSearch;
}

class wSearch : public QFrame
{
    Q_OBJECT

public:
    explicit wSearch(QWidget *parent = 0);
    void setEnv(logger* pLog, indexer* pIndexer);
    ~wSearch();

protected:
    logger* m_pLog;
    indexer* m_pIndexer;
    searcher* m_pSearcher;

private slots:
    void on_eSearchTerm_textChanged(const QString &arg1);
    void on_commandLinkButtonSearch_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void OnCfgChanged(QString sCfgEntryName, QString sCfgEntryValue);
    void fillFields();
    void on_actionInfo();
    void on_actionOpen();

    void on_commandLinkButtonClear_clicked();

    void on_eSearchTerm_returnPressed();

private:
    Ui::wSearch *ui;
};

#endif // WSEARCH_H
