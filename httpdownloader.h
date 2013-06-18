#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <filedownloader.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QByteArray>

class HttpDownloader : public FileDownloader
{
public:
    HttpDownloader(QNetworkAccessManager *accessManager , QObject *parent = 0);

    bool downloading;

    bool downloadFile(QUrl url, quint64 start = 0, quint64 size = 0, int chunkSize = -1, void* ptr = 0);
    quint64 checkFileSize(QUrl url);
    bool isDownloading();

protected:
    bool stopping = false;

public slots:
    void cancelDownload();

};

#endif // HTTPDOWNLOADER_H
