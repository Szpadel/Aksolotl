#include "task.h"

Task::Task(DownloadManager *dm, MetadataFile * metadataFile, QObject *parent) :
    QObject(parent)
{
    downloadManager = dm;
    metaFile = metadataFile;

    chunks.resize(metaFile->getChecksumList().size());
    int pos = 0;
    Q_FOREACH(quint16 checksum, metaFile->getChecksumList()) {
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

void Task::writeCorrectData(Chunk *chunk, QByteArray data)
{
    origFile.seek(chunk->possition() * metadataFile()->getFilesize());
    origFile.write(data);
}

void Task::checkChunks()
{
    moveToThread(&thread);
    quint64 chunkSize = metaFile->getChunkSize();
    int oldProgress = 0;
    setProgress(0);

    Q_FOREACH(Chunk* chunk, chunks){

        bool isExists = origFile.seek(chunk->possition()*chunkSize);
        if(! isExists) {
            chunk->setStatus(Chunk::MISSING);
            tchunksMissing++;
        }else {
            QByteArray data = origFile.read(chunkSize);
            if(qChecksum(data, data.size()) != chunk->checksum()) {
                chunk->setStatus(Chunk::CORRUPTED);
                tchunksCorrupted++;
            } else {
                chunk->setStatus(Chunk::OK);
                tchunksOk++;
            }
        }

        // zapobieganie szybkiemu emitowaniu sygnalow
        int progress = chunk->possition()*100/chunks.size();
        if(progress != oldProgress) {
            setProgress(progress);
            oldProgress = progress;
        }
    }
    emit taskStatusChanged(this);
    if(tchunksCorrupted != 0 || tchunksMissing != 0) {
        setTaskStatus(DOWNLOADING);
        downloadManager->addTask(this);
    }
}

const QVector<Chunk*> Task::getChunks()
{
    return chunks;
}

void Task::changeChunkStatus(Chunk& chunk, Chunk::Status status)
{
    chunk.setStatus(status);
}

Task::TaskStatus Task::getTaskStatus()
{
    return taskStatus;
}

void Task::start()
{
    setTaskStatus(CHECKING);
    checkChunks();
}

void Task::stop()
{
    setTaskStatus(STOPPED);
}

void Task::setProgress(int progress)
{
    this->tprogress = progress;
    emit progressChanged(progress);
}

void Task::setTaskStatus(TaskStatus status){
    taskStatus = status;
    emit taskStatusChanged(this);
}
