#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QThread>
#include <QtCore>
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
        ERROR,
        DONE
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
    const QVector<Chunk*> getChunks();
    void changeChunkStatus(Chunk& chunk, Chunk::Status status);
    TaskStatus getTaskStatus();

    void writeCorrectData(Chunk* chunk, QByteArray data);

    void setTaskStatus(TaskStatus status);

protected:
    MetadataFile* metaFile;
    QFile origFile;
    DownloadManager* downloadManager;
    QVector<Chunk*> chunks;
    TaskStatus taskStatus;
    QThread thread;
    int tprogress = 0;
    int tchunksOk = 0;
    int tchunksCorrupted = 0;
    int tchunksMissing = 0;

    void setProgress(int progress);
    
signals:
    void chunkChanged(Task* task, Chunk* chunk, Chunk::Status oldStatus);
    void taskStatusChanged(Task *task);
    void progressChanged(int progress);

public slots:
    void start();
    void stop();
    
};

#endif // TASK_H
