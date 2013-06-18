#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
    qNetworkAccessManager = new QNetworkAccessManager(this);
}

void DownloadManager::addTask(Task* task)
{
    if(tasks.contains(task))
    {
        return;
    }

    connect(task, SIGNAL(chunkChanged(Task*,Chunk*,Chunk::Status)),
            this, SLOT(chunkChanged(Task*,Chunk*,Chunk::Status)));

    tasks.insert(task, optimizeChunks(task));
    nextMirror.insert(task, task->metadataFile()->getMirrorList().begin());
    QList<QThread*> taskthreads;
    for(int a = 0; a < 4; a++) {
        taskthreads.append(new QThread());
    }
    threads.insert(task, taskthreads);

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
    }
    // TODO: zatrzymywanie zadan
}

void DownloadManager::chunkChanged(Task *task, Chunk *chunk, Chunk::Status oldStatus)
{

}

void DownloadManager::chunkDownloaded(QByteArray chunkData, void *badChunksSpace)
{
    BadChunksSpace* bcs = (BadChunksSpace*) badChunksSpace;
    if(qChecksum(chunkData, chunkData.size()) != bcs->incorrectChunks.first()->checksum()) {
        bcs->fileDownloader->cancelDownload();
        startDownloader(bcs);
    }else {
        bcs->task->writeCorrectData(bcs->incorrectChunks.first(), chunkData);
        bcs->incorrectChunks.pop_front();

        if(bcs->incorrectChunks.size() == 0) {
            delete bcs->fileDownloader;
            Task *t = bcs->task;
            (*tasks.find(t)).removeAll((*bcs));
            delete bcs;
            startNextBCS(t);
        }
    }
}

bool DownloadManager::startDownloader(BadChunksSpace *bcs)
{
    if(bcs->fileDownloader == NULL) {
        QThread* th = NULL;
        Q_FOREACH(QThread* thread, threads.value(bcs->task)) {
            if(! thread->isRunning()) {
                th = thread;
                break;
            }
        }
        if(th == NULL) {
            return false;
        }

        bcs->fileDownloader = new HttpDownloader(qNetworkAccessManager, this);
        bcs->fileDownloader->setThread(th);
    }
    QList<QUrl>::const_iterator mirror = nextMirror.value(bcs->task);
    quint64 chunksize = bcs->task->metadataFile()->getChunkSize();

    connect(bcs->fileDownloader, SIGNAL(chunkDownloaded(QByteArray,void*)),
            this, SLOT(chunkDownloaded(QByteArray,void*)));

    bcs->fileDownloader->downloadFile(*mirror,
                                      bcs->incorrectChunks.first()->possition() * chunksize,
                                      bcs->incorrectChunks.size() * chunksize,
                                      chunksize,
                                      bcs
                                      );

    if(mirror == bcs->task->metadataFile()->getMirrorList().end()) {
        nextMirror.insert(bcs->task, bcs->task->metadataFile()->getMirrorList().begin());
    }else {
        nextMirror.insert(bcs->task, ++mirror);
    }
    return true;
}

void DownloadManager::startNextBCS(Task *task)
{
    Q_FOREACH(BadChunksSpace bcs, tasks.value(task)) {
        if(bcs.fileDownloader == NULL) {
            if( ! startDownloader(&bcs)) {
                return;
            }
        }
    }
}

void DownloadManager::startDownloading(Task *task)
{
    for(int a = 0; a < 4; a++) {
        startNextBCS(task);
    }
}

QList<DownloadManager::BadChunksSpace> DownloadManager::optimizeChunks(Task *task)
{
    QList<BadChunksSpace> badChunksSpaces;

    BadChunksSpace* bcs = NULL;

    Q_FOREACH(Chunk* chunk, task->getChunks()) {
        if(chunk->getStatus() != Chunk::OK) {
            if(bcs == NULL) {
                bcs = new BadChunksSpace();
                bcs->task = task;
            }
            bcs->incorrectChunks.append(chunk);
        }else {
            badChunksSpaces.append(*bcs);
            bcs = NULL;
        }
    }
    if(bcs != NULL) {
        badChunksSpaces.append(*bcs);
    }

    return badChunksSpaces;
}
