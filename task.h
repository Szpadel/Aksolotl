#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QThread>

class Task;

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

    Task(DownloadManager *dm, MetadataFile* metadataFile, QObject *parent = 0);
    MetadataFile* metadataFile();    

    int progress();
    int chunksOk();
    int chunksCorrupted();
    int chunksMissing();
    QString getOrigFileLocation();
    void setOrigFileLocation(QString location);

    /**
     * @brief checkChunks wymaga aby plik byl wczesniej otwarty
     */
    void checkChunks();
    const QVector<Chunk> getChunks();
    void changeChunkStatus(const Chunk& chunk, Chunk::Status status);
    TaskStatus getTaskStatus();

    void start();
    void stop();

protected:
    MetadataFile* metaFile;
    QFile origFile;
    DownloadManager* downloadManager;
    QVector<Chunk*> chunks;
    TaskStatus taskStatus;
    QThread thread;
    int progress = 0;
    int chunksOk = 0;
    int chunksCorrupted = 0;
    int chunksMissing = 0;

    void setProgress(int progress);
    quint64 generateCheckSum();
    
signals:
    void chunkChanged(Task* task, Chunk* chunk, Chunk::Status oldStatus);
    void taskStatusChanged(Task *task);
    void progressChanged(int progress);

public slots:
    
};

#endif // TASK_H
