#ifndef WSETTINGS_H
#define WSETTINGS_H

#include <QFrame>
#include "logger.h"
#include "indexer.h"

namespace Ui
{
    class wSettings;
}

class wSettings : public QFrame
{
    Q_OBJECT

public:
    explicit wSettings(QWidget *parent = 0);
    ~wSettings();
    void setEnv(logger* pLog, indexer* pIndexer);

protected:
    logger* m_pLog;
    indexer* m_pIndexer;
    void refreshtableViewDirs(int iCfgEntriesLen);

private slots:
    void on_toolButtonAdd_clicked();
    void on_toolButtonDel_clicked();
    void on_tableViewDirs_clicked(const QModelIndex &index);

private:
    Ui::wSettings *ui;
};

#endif // WSETTINGS_H
