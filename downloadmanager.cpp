#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
}

void DownloadManager::addTask(Task task)
{
    tasks.append(task);
}
void DownloadManager::taskStatusChanged(Task *task)
{
}
void DownloadManager::chunkChanged(Task *task, Chunk *chunk)
{
}

QList<incorrectChunkInfo> DownloadManager::optimizeChunks(Task &task)
{
    int sizeOfIncorrect = 0;
    QList<incorrectChunkInfo> badChunksSpaces;
    foreach (Chunk chunk, task.getChunks()) {
        if(chunk.getStatus() != chunk.OK)
        {
            sizeOfIncorrect++;
        }
        else if(sizeOfIncorrect!=0)
        {
            BadChunksSpace badChunksSpace;
            badChunksSpace.firstIncorrectChunk = chunk;
            badChunksSpace.sizeOfCorruption = sizeOfIncorrect;

            badChunksSpaces.push_back(badChunksSpace);
            longestIncorrect=0;
        }
    }
    return badChunksSpaces;
}
