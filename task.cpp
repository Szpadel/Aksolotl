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
    return progress;
}

int Task::chunksOk()
{
    return chunksOk;
}

int Task::chunksCorrupted()
{
    return chunksCorrupted;
}

int Task::chunksMissing()
{
    return chunksMissing;
}

QString Task::origFileLocation()
{
    return origFile.fileName();
}

void Task::setOrigFileLocation(QString location)
{
    if(origFile.isOpen()){
        origFile.close();
    }

    progress = 0;
    chunksCorrupted = 0;
    chunksMissing = 0;
    chunksOk = 0;

    origFile.setFileName(location);
    origFile.open(QIODevice::ReadWrite);

    Q_FOREACH(Chunk chunk, chunks){
        chunk.setStatus(Chunk::UNKNOWN);
    }
}

void Task::checkChunks()
{
    moveToThread(&thread);
    quint64 chunkSize = metaFile->getChunkSize();
    Q_FOREACH(Chunk* chunk, chunks){
        progress = chunk->checksum()/chunkSize*100;
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

QVector<Chunk> Task::getChunks()
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
    if(Chunk::getStatus() == Chunk::OK){
        //TO DO
    }
}

void Task::stop()
{

}

void Task::setProgress(int progress)
{
    this->progress = progress;
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
