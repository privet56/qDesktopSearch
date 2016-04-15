#ifndef WSEARCH_H
#define WSEARCH_H

#include <QFrame>

namespace Ui {
class wSearch;
}

class wSearch : public QFrame
{
    Q_OBJECT

public:
    explicit wSearch(QWidget *parent = 0);
    ~wSearch();

private slots:
    void on_eSearchTerm_textChanged(const QString &arg1);

    void on_commandLinkButtonSearch_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::wSearch *ui;
};

#endif // WSEARCH_H
