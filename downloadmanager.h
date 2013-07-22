#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QThread>

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

    void startDownloading(Task *task);
    void stopDownloading(Task *task);
    void pauseDownloading(Task *task);

protected:
    struct BadChunksSpace
    {
        FileDownloader *fileDownloader;
        QList<Chunk*> incorrectChunks;
        Task* task = NULL;
    };

    QMap<Task*, QList<BadChunksSpace*>> tasks;
    QMap<Task*, QList<QUrl>::const_iterator> nextMirror;
    QMap<Task*, QList<QThread*>> threads;

    QNetworkAccessManager* qNetworkAccessManager;
    QList<FileDownloader> fileDownloaders;

    QList<BadChunksSpace*> optimizeChunks(Task *task);
    bool startDownloader(BadChunksSpace* bcs);
    void startNextBCS(Task* task);

signals:
    
public slots:
    void taskStatusChanged(Task &task);
    void chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus);
    void chunkDownloaded(QByteArray chunkData, void *badChunksSpace);
    
};

#endif // DOWNLOADMANAGER_H
