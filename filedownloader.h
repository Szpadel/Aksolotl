#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QString>
#include <QThread>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    enum ErrorType
    {
        NO_ERROR,
        FILE_NOT_FOUND,
        WRONG_RESPONSE,
        PARTIAL_NOT_SUPPORTED,
        UNKNOWN_ERROR
    };

    FileDownloader(QNetworkAccessManager *accessManager , QObject *parent = 0) :
        QObject(parent)
    {
        this->accessManager = accessManager;
    }

    void setThread(QThread* th) {
        thread = th;
    }

    virtual bool downloadFile(QUrl url, quint64 start = 0, quint64 size = 0, int chunkSize = 0, void* ptr = 0) = 0;
    virtual quint64 checkFileSize(QUrl url) = 0;

    ErrorType getError()
    {
        return errorStatus;
    }

    virtual bool isDownloading() = 0;


protected:
    QNetworkAccessManager *accessManager;
    ErrorType errorStatus = NO_ERROR;
    QThread* thread = NULL;
    
signals:
    void chunkDownloaded(QByteArray chunkData, void* ptr);
    void fileDownloaded();
    void error(ErrorType e, QString description);

public slots:
    virtual void cancelDownload() = 0;
    
};

#endif // FILEDOWNLOADER_H
