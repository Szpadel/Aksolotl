#include "task.h"

Task::Task(DownloadManager *dm, MetadataFile * metadataFile, QObject *parent) :
    QObject(parent)
{
    downloadManager = dm;
    metaFile = metadataFile;

    chunks.resize(metaFile->chunksChecksum.size());
    int pos = 0;
    Q_FOREACH(quint16 checksum, metaFile->chunksChecksum) {
        chunks.replace(pos, new Chunk(pos,checksum, this));
        pos++;
    }
}

MetadataFile* Task::metadataFile()
{
    return metaFile;
}

int Task::progress()
{
    return tprogress;
}

int Task::chunksOk()
{
    return tchunksOk;
}

int Task::chunksCorrupted()
{
    return tchunksCorrupted;
}

int Task::chunksMissing()
{
    return tchunksMissing;
}

QString Task::getOrigFileLocation()
{
    return origFile.fileName();
}

void Task::setOrigFileLocation(QString location)
{
    if(origFile.isOpen()){
        origFile.close();
    }

    tprogress = 0;
    tchunksCorrupted = 0;
    tchunksMissing = 0;
    tchunksOk = 0;

    origFile.setFileName(location);
    origFile.open(QIODevice::ReadWrite);
    taskStatus = CHECKING;

    Q_FOREACH(Chunk* chunk, chunks){
        chunk->setStatus(Chunk::UNKNOWN);
    }
}

void Task::checkChunks()
{
    moveToThread(&thread);
    quint64 chunkSize = metaFile->getChunkSize();
    Q_FOREACH(Chunk* chunk, chunks){
        tprogress = chunk->checksum()/chunkSize*100;
        emit progressChanged(tprogress);

        if(chunk->checksum() == chunkSize){
            tchunksOk++;
            tchunksCorrupted = 0;
            tchunksMissing = 0;
        }

        bool isExists = origFile.seek(chunk->possition()*chunkSize);
        if(! isExists) {
            chunk->setStatus(Chunk::MISSING);
        }else {
            QByteArray data = origFile.read(chunkSize);
            if(qChecksum(data, data.size()) != chunk->checksum()) {
                chunk->setStatus(Chunk::CORRUPTED);
            } else {
                chunk->setStatus(Chunk::OK);
            }
        }
    }
}

const QVector<Chunk*> Task::getChunks()
{
    return chunks;
}

void Task::changeChunkStatus(const Chunk& chunk, Chunk::Status status)
{
    chunk.setStatus(status);
}

TaskStatus Task::getTaskStatus()
{
    return taskStatus;
}

void Task::start()
{
    if(TaskStatus == CHECKING){
        DownloadManager.addTask(new Task(DownloadManager, metaFile, parent));
    }
    emit taskStatusChanged(this);
}

void Task::stop()
{
    taskStatus = STOPPED;
    emit taskStatusChanged(this);
}

void Task::setProgress(int progress)
{
    this->tprogress = progress;
}

quint64 Task::generateCheckSum()
{
    quint64 chunkSum = 0;
    if(origFile.isOpen()){
        Q_FOREACH(Chunk* chunk, chunks){
            chunkSum = qChecksum(chunk, metaFile->getChunkSize());
        }
    }
    return chunkSum;
}
