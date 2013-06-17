#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QList>

#include <task.h>
#include <chunk.h>

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);

    void addTask(Task task);
    QList<Task*> getTasks();


protected:
    QList<Task*> tasks;
    QList<incorrectChunkInfo> optimizeChunks(Task *task);
    struct BadChunksSpace
    {
        Chunk* firstIncorrectChunk;
        int sizeOfCorruption;
    };

signals:
    
public slots:
    void taskStatusChanged(Task &task);
    void chunkChanged(Task *task, Chunk* chunk, Chunk::Status oldStatus);
    
};

#endif // DOWNLOADMANAGER_H
