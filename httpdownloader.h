#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <filedownloader.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>

class HttpDownloader : public FileDownloader
{
public:
    HttpDownloader(QNetworkAccessManager *accessManager , QObject *parent = 0);

    bool downloadFile(QUrl url, quint64 start = 0, quint64 end = 0, quint64 chunkSize = 0);
    quint64 checkFileSize(QUrl url);
    bool isDownloading();

public slots:
    void cancelDownload();

};

#endif // HTTPDOWNLOADER_H
