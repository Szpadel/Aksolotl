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

    bool downloadFile(QUrl url, quint64 start = 0, quint64 size = 0, int chunkSize = -1);
    quint64 checkFileSize(QUrl url);
    bool isDownloading();

public slots:
    void cancelDownload();

};

#endif // HTTPDOWNLOADER_H
