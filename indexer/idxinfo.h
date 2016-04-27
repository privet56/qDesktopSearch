#ifndef IDXINFO_H
#define IDXINFO_H

#include <QObject>

class IdxInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dir2Index READ dir2Index WRITE setDir2Index)
    Q_PROPERTY(QString indexDir READ indexDir WRITE setIndexDir)

    Q_PROPERTY(size_t indexDirSize READ indexDirSize WRITE setIndexDirSize)
    Q_PROPERTY(size_t indexDirNrOfFiles READ indexDirNrOfFiles WRITE setIndexDirNrOfFiles)

    Q_PROPERTY(size_t indexNrOfFilesInIndex READ indexNrOfFilesInIndex WRITE setIndexNrOfFilesInIndex)
    Q_PROPERTY(size_t indexNrOfIndexedFilesInCurrentLoop READ indexNrOfIndexedFilesInCurrentLoop WRITE setIndexNrOfIndexedFilesInCurrentLoop)
    Q_PROPERTY(bool indexIsNew READ indexIsNew WRITE setIndexIsNew)

public:
    explicit IdxInfo(QObject *parent = 0);

    void fill(QString dir2Index);

//GET
    QString dir2Index();
    QString indexDir();

    size_t indexDirSize();
    size_t indexDirNrOfFiles();

    bool indexIsNew();
    size_t indexNrOfFilesInIndex();
    size_t indexNrOfIndexedFilesInCurrentLoop();

//SET
    void setDir2Index(QString s);
    void setIndexDir(QString s);

    void setIndexDirSize(size_t i);
    void setIndexDirNrOfFiles(size_t i);

    void setIndexIsNew(bool b);
    void setIndexNrOfFilesInIndex(size_t i);
    void setIndexNrOfIndexedFilesInCurrentLoop(size_t i);

protected:
    QString m_dir2Index;
    QString m_indexDir;
    size_t m_indexDirSize;
    size_t m_indexDirNrOfFiles;
    bool m_indexIsNew;
    size_t m_indexNrOfFilesInIndex;
    size_t m_indexNrOfIndexedFilesInCurrentLoop;

    void fillDirInfo(QString dir2Index);

signals:
    void fillIdxInfo(IdxInfo* idxi);

public slots:
};

#endif // IDXINFO_H
