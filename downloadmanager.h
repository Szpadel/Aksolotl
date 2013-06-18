#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QList>
class DownloadManager;
#include <task.h>
#include <chunk.h>
#include <httpdownloader.h>

#include <filedownloader.h>

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);

    void addTask(Task *task);
    QList<Task*> getTasks();


protected:
    struct BadChunksSpace
    {
        FileDownloader *fileDownloader;
        QList<Chunk*> incorrectChunks;
        Task* task = NULL;
    };

    QNetworkAccessManager qNetworkAccessManager;
    QList<Task*> tasks;
    QList<BadChunksSpace> optimizeChunks(Task &task);
    QList<FileDownloader> fileDownloaders;

    void startDownloading(Task *task);
    void stopDownloading(Task *task);
    void pauseDownloading(Task *task);


signals:
    
public slots:
    void taskStatusChanged(Task &task);
    void chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus);
    void chunkDownloaded(QByteArray chunkData, void *badChunksSpace);
    
};

#endif // DOWNLOADMANAGER_H
