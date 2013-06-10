#ifndef METADATAFILE_H
#define METADATAFILE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QUrl>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

#include <exception>
#include <cmath>

class MetadataFileException : public std::exception {

};

class MetadataFile : public QObject
{
    Q_OBJECT
public:

    enum ErrorReason {
        NO_ERROR,
        CANT_OPEN,
        FILE_CORRUPTED,
        FILE_WRONG_VERSION
    };

    enum FileStatus {
        CLOSED,
        OPEN,
        LOADED
    };

    explicit MetadataFile(QObject *parent = 0);

    /**
     * Otwiera i odczytuje zawartosc pliku
     *
     * @brief open
     * @param metadataFilename scie¿ka do pliku
     * @return czy siê uda³o
     */
    bool open(QString metadataFilename);
    void close();

    bool isFileOpened();
    bool isLoaded();
    bool isFileCorrupted();

    /**
     * Uaktualnia plik zmodyfikowany po odczytaniu
     * lub zapisuje nowy jesli nie istnieje
     *
     * @brief save
     * @param metadataFile nazwa pliku
     * @return czy sie uda³o
     */
    bool save(QString metadataFilename = "");
    
    QString getFilename();
    void setFilename(QString filename);

    quint64 getFilesize();
    void setFilesize(quint64 size);

    QString getDescription();
    void setDescription(QString description);

    const QList<QUrl> &getMirrorList();
    void setMirrors(const QList<QUrl> &mirrors);
    void addMirror(const QUrl &mirror);
    void addMirrors(const QList<QUrl> &mirrors);
    void removeMirror(const QUrl &mirror);
    void removeAllMirrors();
    bool containsMirror(const QUrl &mirror);

    const QList<QUrl> &getRepositoryList();
    void setRepositories(const QList<QUrl> &repositories);
    void addRepository(const QUrl &repository);
    void addRepositories(const QList<QUrl> &repositories);
    void removeRepository(const QUrl &repository);
    void removeAllRepositories();
    bool containsRepository(const QUrl &repository);

    uchar getChunkSizeLevel();
    quint64 getChunkSize();
    void setChunkSizeLevel(uchar chunkSizeLevel);

    void addChunk(quint16 checksum);
    quint16 checksumAt(int i);
    void removeAllChunks();


protected:

    bool readData();

    QFile metadataFile;
    QByteArray rawData;
    qint32 chunksize;
    ErrorReason error = NO_ERROR;
    FileStatus fileStatus = CLOSED;

    // metadata file fields BEGIN
    // header
    QString filename;
    quint64 filesize;
    QString description;
    QList<QUrl> mirrors;
    QList<QUrl> repositories;
    uchar chunkSizeLevel;
    // end of header

    quint16 headerChecksum;

    QList<quint16> chunksChecksum;

    quint16 dataChecksum;
    // metadata file fields END

signals:
    
public slots:
    
};

#endif // METADATAFILE_H
