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
        disconnect(task, SIGNAL(taskStatusChanged(Task*, Chunk*, Chunk::Status)));
        disconnect(task, SIGNAL(chunkChanged(Task*)));
        tasks.removeOne(task);
    }
}

void DownloadManager::chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus)
{
    //if(chunk->getStatus() == Chunk::OK)
    //{
        //((BadChunksSpace*)badChunksSpace)->task-> chunk sie poprawil
    //    ((BadChunksSpace*)badChunksSpace)->incorrectChunks.removeOne(&chunk);
    //}
}

void DownloadManager::chunkDownloaded(QByteArray chunkData, void *badChunksSpace)
{
    Chunk* downloadedChunk = ((BadChunksSpace*)badChunksSpace)->incorrectChunks.at(0);
    if(downloadedChunk->checksum() == qChecksum(chunkData,chunkData.size()))
    {
        // TODO: WPISAĆ NAZWĘ FUNKCJI
        ((BadChunksSpace*)badChunksSpace)->task->dupujDane(chunkData);
    }

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
    BadChunksSpace *badChunkSpace;

    foreach (Chunk *chunk, task.getChunks()) {

        if(lastChunk != NULL && chunk->getStatus() != Chunk::OK)
        {
            if(lastChunk->getStatus() != Chunk::OK)
            {
                badChunkSpace->incorrectChunks.push_back(&chunk);
            }
            else
            {
                badChunkSpace->fileDownloader = new FileDownloader;
                connect(badChunkSpace->fileDownloader,SIGNAL(chunkDownloaded(QByteArray)),
                        this, chunkDownloaded(QByteArray));

                badChunkSpace->task=task;
                badChunkSpace->incorrectChunks.push_back(&chunk);

                badChunksSpaces.push_back(badChunkSpace);
                badChunkSpace = new badChunkSpace;
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
