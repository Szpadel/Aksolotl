#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QFile>
#include <QVector>

#include <metadatafile.h>
#include <downloadmanager.h>
#include <chunk.h>

class Task : public QObject
{
    Q_OBJECT
public:
    enum TaskStatus {
        STOPPED,
        DOWNLOADING,
        CHECKING,
        ERROR
    };

    Task(DownloadManager *dm, QObject *parent = 0);
    MetadataFile* metadataFile();
    int progress();
    int chunksOk();
    int chunksCorrupted();
    int chunksMissing();
    QString orgFileLocation();
    void checkChunks();
    const QVector<Chunk> getChunks();
    void changeChunkStatus(const Chunk& chunk, Chunk::Status);
    void startDOwnload();
    void stopDownload();


protected:
    MetadataFile* metaFile;
    QFile origFile;
    DownloadManager* downloadManager;
    QVector<Chunk> chunks;
    
signals:
    void chunkChanged(Task* task, Chunk* chunk);
    void taskStatusChanged(Task *task);
    void progressChanged(int progress);

public slots:
    
};

#endif // TASK_H
