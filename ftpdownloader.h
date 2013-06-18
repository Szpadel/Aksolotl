#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <filedownloader.h>

#include<QFtp>
#include<QObject>
#include<QUrl>
#include<QNetworkAccessManager>


class FtpDownloader : public FileDownloader
{
public:
    FtpDownloader(QNetworkAccessManager *accessManager , QObject *parent = 0);

    bool isDownloading;

    bool downloadFile(QUrl url, quint64 start = 0, quint64 size = 0, int chunkSize = -1);
    quint64 checkFileSize(QUrl url);
    bool isDownloading();

public slots:
    void cancelDownload();
};

#endif // FTPDOWNLOADER_H
