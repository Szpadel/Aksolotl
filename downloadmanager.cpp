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

    connect(task, SIGNAL(chunkChanged(Task*,Chunk*,Chunk::Status)),
            this, SLOT(chunkChanged(Task*,Chunk*,Chunk::Status)));

    tasks.append(task);

    if(task->getTaskStatus() == Task::DOWNLOADING)
    {
        this->startDownloading(task);
    }
}
void DownloadManager::taskStatusChanged(Task &task)
{
    if(task.getTaskStatus() == Task::DOWNLOADING)
    {
        this->startDownloading(&task);
    } else
    {
        disconnect(&task, SIGNAL(chunkChanged(Task*,Chunk*,Chunk::Status)));
        tasks.removeOne(&task);
    }
}

void DownloadManager::chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus)
{
    if(chunk->getStatus() == Chunk::OK)
    {
        //((BadChunksSpace*)badChunksSpace)->task-> chunk sie poprawil
    //    ((BadChunksSpace*)badChunksSpace)->incorrectChunks.removeOne(&chunk);
    }
}

void DownloadManager::chunkDownloaded(QByteArray chunkData, void *badChunksSpace)
{
    Chunk* downloadedChunk = ((BadChunksSpace*)badChunksSpace)->incorrectChunks.at(0);
    if(downloadedChunk->checksum() == qChecksum(chunkData,chunkData.size()))
    {
        // TODO: WPISAĆ NAZWĘ FUNKCJI
        //((BadChunksSpace*)badChunksSpace)->task->dupujDane(chunkData);
    }

}

void DownloadManager::startDownloading(Task *task)
{
    QList<BadChunksSpace> badChunksSpaces = optimizeChunks(*task);

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

QList<DownloadManager::BadChunksSpace> DownloadManager::optimizeChunks(Task &task)
{
    QList<BadChunksSpace> badChunksSpaces;

    Chunk *lastChunk = NULL;
    BadChunksSpace *badChunksSpace = NULL;

    foreach (Chunk *chunk, task.getChunks()) {

        if(lastChunk != NULL && chunk->getStatus() != Chunk::OK)
        {
            if(lastChunk->getStatus() != Chunk::OK)
            {
                badChunksSpace->incorrectChunks.push_back(chunk);
            }
            else
            {
                badChunksSpace->fileDownloader = new HttpDownloader(&qNetworkAccessManager);
                connect(badChunksSpace->fileDownloader,SIGNAL(chunkDownloaded(QByteArray)),
                        this, SLOT(chunkDownloaded(QByteArray,void*)));

                badChunksSpace->task=&task;
                badChunksSpace->incorrectChunks.push_back(chunk);

                badChunksSpaces.push_back(*badChunksSpace);
                badChunksSpace = new BadChunksSpace();
            }
        }
    }
    return badChunksSpaces;
}
