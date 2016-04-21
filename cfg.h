#ifndef CFG_H
#define CFG_H

#include <QObject>
#include <QSettings>

class cfg : public QObject
{
    Q_OBJECT

    QString m_sDefault;
    QString m_sPathName;
    QSettings* m_pSettings;
public:
    explicit cfg(QObject *parent = 0);
    QString getValue(QString sCfgEntryName);
    void setValue(QString sCfgEntryName, QString sCfgEntryValue);
    QString replacePlaceHolders(QString s);

    static bool mkdirfull(QString sDir);

signals:
    void cfgChanged(QString sCfgEntryName, QString sCfgEntryValue);

public slots:
};

#endif // CFG_H
