#include "metadatafile.h"

MetadataFile::MetadataFile(QObject *parent) :
    QObject(parent)
{
}

bool MetadataFile::open(QString metadataFilename)
{
    if(fileStatus != CLOSED) {
        close();
    }
    metadataFile.setFileName(metadataFilename);
    metadataFile.open(QIODevice::ReadWrite);
    if(! metadataFile.isReadable()) {
        error = CANT_OPEN;
        return false;
    }
    fileStatus = OPEN;
    rawData = metadataFile.readAll();

    return readData();
}

bool MetadataFile::readData()
{
    QDataStream data(rawData);
    char sign[2];
    data.readRawData(&sign[0], 2);

    if(sign[0] != 'A' || sign[1] != 'X') {
        error = FILE_CORRUPTED;
        return false;
    }

    qint16 version;
    data >> version;

    if(version != 0) {
        error = FILE_WRONG_VERSION;
        return false;
    }

    QByteArray header;
    QDataStream headerStream(&header, QIODevice::ReadWrite);

    data >> header >> headerChecksum;

    if(qChecksum(header, header.size()) != headerChecksum) {
        error = FILE_CORRUPTED;
        return false;
    }

    headerStream >> filename >>
            filesize >>
            description >>
            mirrors >>
            repositories >>
            chunkSizeLevel;


    QByteArray chunksData;
    QDataStream chunksStream(&chunksData, QIODevice::ReadWrite);
    QByteArray chunksDataComp;

    data >> chunksDataComp >>
            dataChecksum;

    if(qChecksum(chunksDataComp, chunksDataComp.size()) != dataChecksum) {
        error = FILE_CORRUPTED;
        return false;
    }

    chunksData = qUncompress(chunksDataComp);

    chunksStream >> chunksChecksum;

    chunksize = pow(2, chunkSizeLevel)*1024;

    if(chunksChecksum.size() != ceil((double)filesize/chunksize)) {
        error = FILE_CORRUPTED;
        return false;
    }

    fileStatus = LOADED;
    return true;
}

void MetadataFile::close()
{
    metadataFile.close();
    rawData.clear();
    error = NO_ERROR;
    fileStatus = CLOSED;

    filename = "";
    filesize = 0;
    description = "";
    mirrors.clear();
    repositories.clear();
    chunkSizeLevel = 0;
}

bool MetadataFile::isFileCorrupted()
{
    return error == FILE_CORRUPTED;
}

bool MetadataFile::isFileOpened()
{
    return fileStatus == OPEN || fileStatus == LOADED;
}

bool MetadataFile::isLoaded()
{
    return fileStatus == LOADED;
}

bool MetadataFile::save(QString metadataFilename)
{
    if(metadataFilename == "") {
        metadataFilename = this->metadataFile.fileName();
    }

    rawData.clear();
    QDataStream data(&rawData, QIODevice::WriteOnly);

    data.writeRawData("AX", 2);
    data << (qint16) 0;

    QByteArray header;
    header.clear();
    QDataStream headerData(&header, QIODevice::WriteOnly);
    headerData << filename <<
                filesize <<
                description <<
                mirrors <<
                repositories <<
                chunkSizeLevel;

    data << header <<
               qChecksum(header, header.size());

    QByteArray chunksData;
    QByteArray chunksDataComp;
    QDataStream chunks(&chunksData, QIODevice::WriteOnly);

    chunks << chunksChecksum;

    chunksDataComp = qCompress(chunksData, 9);

    data << chunksDataComp <<
               qChecksum(chunksDataComp, chunksDataComp.size());

    if(fileStatus == CLOSED) {
        metadataFile.setFileName(metadataFilename);
        metadataFile.open(QIODevice::ReadWrite);
    }
    if(! metadataFile.isWritable()) {
        qDebug() << "metadata file is not writable!";
    }
    metadataFile.seek(0);
    metadataFile.write(rawData);
    if(metadataFile.error() != QFile::FileError::NoError) {
        qDebug() << "Metadata save error: " <<
                    metadataFile.errorString();
        return false;
    }
    metadataFile.resize(metadataFile.pos());
    return true;
}

QString MetadataFile::getFilename()
{
    return this->filename;
}

void MetadataFile::setFilename(QString filename)
{
    this->filename = filename;
}

quint64 MetadataFile::getFilesize()
{
    return filesize;
}

void MetadataFile::setFilesize(quint64 size)
{
    this->filesize = size;
}

QString MetadataFile::getDescription()
{
    return description;
}

void MetadataFile::setDescription(QString description)
{
    this->description = description;
}

const QList<QUrl> &MetadataFile::getMirrorList()
{
    return mirrors;
}

void MetadataFile::setMirrors(const QList<QUrl> &mirrors)
{
    this->mirrors = mirrors;
}

void MetadataFile::addMirror(const QUrl &mirror)
{
    mirrors.append(mirror);
}

void MetadataFile::addMirrors(const QList<QUrl> &mirrors)
{
    Q_FOREACH(QUrl mirror, mirrors) {
        this->mirrors.append(mirror);
    }
}

void MetadataFile::removeMirror(const QUrl &mirror)
{
    this->mirrors.removeAll(mirror);
}

void MetadataFile::removeAllMirrors()
{
    this->mirrors.clear();
}

bool MetadataFile::containsMirror(const QUrl &mirror)
{
    return this->mirrors.contains(mirror);
}

const QList<QUrl> &MetadataFile::getRepositoryList()
{
    return repositories;
}

void MetadataFile::setRepositories(const QList<QUrl> &repositories)
{
    this->repositories = repositories;
}

void MetadataFile::addRepository(const QUrl &repository)
{
    this->repositories.append(repository);
}

void MetadataFile::addRepositories(const QList<QUrl> &repositories)
{
    Q_FOREACH(QUrl repo, repositories) {
        this->repositories.append(repo);
    }
}

void MetadataFile::removeRepository(const QUrl &repository)
{
    this->repositories.removeAll(repository);
}

void MetadataFile::removeAllRepositories()
{
    this->repositories.clear();
}

bool MetadataFile::containsRepository(const QUrl &repository)
{
    return this->repositories.contains(repository);
}

uchar MetadataFile::getChunkSizeLevel()
{
    return chunkSizeLevel;
}

void MetadataFile::setChunkSizeLevel(uchar chunkSizeLevel)
{
    this->chunkSizeLevel = chunkSizeLevel;
    this->chunksize = pow(2, chunkSizeLevel)*1024;
}


quint64 MetadataFile::getChunkSize()
{
    return chunksize;
}

void MetadataFile::addChunk(quint16 checksum)
{
    chunksChecksum.append(checksum);
}

quint16 MetadataFile::checksumAt(int i)
{
    return chunksChecksum.at(i);
}

void MetadataFile::removeAllChunks()
{
    chunksChecksum.clear();
}

const QList<quint16> &MetadataFile::getChecksumList()
{
    return chunksChecksum;
}
