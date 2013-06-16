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

QMap<Chunk*,int> DownloadManager::optimizeChunks(Task &task)
{
    int longestIncorrect = 0;
    QMap<Chunk*,int> badWays;
    foreach (Chunk chunk, task.getChunks()) {
        if(chunk.getStatus() != chunk.OK)
        {
            longestIncorrect++;
        }
        else if(longestIncorrect!=0)
        {
            chunkyWay.insert(chunk,longestIncorrect);
            longestIncorrect=0;
        }
    }
    return badWays;
}
