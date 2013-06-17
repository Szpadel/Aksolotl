#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{

}

void DownloadManager::addTask(Task* task)
{
    if(tasks.contains(task))
    {
        return;
    }

    connect(&task, SIGNAL(taskStatusChanged(Task*, Chunk*, Chunk::Status)),
            this, SLOT(taskStatusChanged(Task*, Chunk*, Chunk::Status)));
    connect(&task, SIGNAL(chunkChanged(Task*)),
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
    } else
    {
        tasks.removeOne(task);
    }
}

void DownloadManager::chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus)
{

}

void DownloadManager::startDownloading(Task *task)
{
    QList<BadChunksSpace> badChunksSpaces = optimizeChunks(task);

    foreach (BadChunksSpace badChunksSpace, badChunksSpaces) {
        QList<QUrl> url = badChunksSpace.task->metadataFile()->getMirrorList();
        int position = badChunksSpace.incorrectChunks.at(0)->possition();
        int incorrectChunks = badChunksSpace.incorrectChunks.size();
        quint64 chunksize = badChunksSpace.task->metadataFile()->getFilesize();

        badChunksSpace.fileDownloader->downloadFile(url.at(0),
                                                    position*chunksize,
                                                    incorrectChunks*chunksize,
                                                    chunksize);
    }
}

QList<BadChunksSpace> DownloadManager::optimizeChunks(Task &task)
{
    QList<BadChunksSpace> badChunksSpaces;

    Chunk *lastChunk = NULL;
    BadChunksSpace *badChunkSpace = new badChunkSpace;

    foreach (Chunk chunk, task.getChunks()) {

        if(chunk.getStatus() != NULL && chunk.getStatus() != Chunk::OK)
        {
            if(lastChunk->getStatus() != Chunk::OK)
            {
                badChunkSpace->incorrectChunks.push_back(&chunk);
            }
            else
            {
                badChunkSpace = new badChunkSpace;
                badChunkSpace->fileDownloader = new FileDownloader;
                badChunkSpace->task=task;
                badChunkSpace->incorrectChunks.push_back(&chunk);

                badChunksSpaces.push_back(badChunkSpace);
            }
            isLastChankCorrect = false;
        }
        else
        {
            isLastChankCorrect = true;
        }
    }
    return badChunksSpaces;
}
