#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{

}

void DownloadManager::addTask(Task task)
{
    connect(  task, SIGNAL(taskStatusChanged(Task*, Chunk*, Chunk::Status)),
            this, SLOT(taskStatusChanged(Task*, Chunk*, Chunk::Status)));
    connect( task, SIGNAL(chunkChanged(Task*)),
            this, SLOT(chunkChanged(Task*)));

    tasks.append(task);

    if(task.getTaskStatus() == Task::DOWNLOADING)
    {
        this->startDownloading(task);
    }
}
void DownloadManager::taskStatusChanged(Task *task)
{
    if(task->getTaskStatus() == Task::DOWNLOADING)
    {
        this->startDownloading(task);
    } else if(task->getTaskStatus() == Task::STOPPED)
    {
        tasks.removeOne(task);
    }
}

void DownloadManager::chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus)
{

}

void DownloadManager::startDownloading(Task *task)
{
    QList<incorrectChunkInfo> badChunksSpaces = optimizeChunks(task);
    QList<QUrl> url = task.metadataFile()->getMirrorList();
    int position = badChunksSpace.firstIncorrectChunk->possition();
    quint64 chunksize = task.metadataFile()->getFilesize();

    foreach (BadChunksSpace badChunksSpace, badChunksSpaces) {
        fileDownloader.downloadFile(
                        url.at(0),
                        chunksize * position,
                        chunksize * badChunksSpace.sizeOfCorruption,
                        chunksize);
    }
}

QList<incorrectChunkInfo> DownloadManager::optimizeChunks(Task &task)
{
    int sizeOfCorruption = 0;
    QList<incorrectChunkInfo> badChunksSpaces;
    foreach (Chunk chunk, task.getChunks()) {
        if(chunk.getStatus() != chunk.OK)
        {
            sizeOfCorruption++;
        }
        else if(sizeOfCorruption!=0)
        {
            BadChunksSpace badChunksSpace;
            badChunksSpace.firstIncorrectChunk = chunk;
            badChunksSpace.sizeOfCorruption = sizeOfCorruption;

            badChunksSpaces.push_back(badChunksSpace);
            sizeOfCorruption=0;
        }
    }
    return badChunksSpaces;
}
