#include "httpdownloader.h"

HttpDownloader::HttpDownloader(QNetworkAccessManager *accessManager, QObject *parent) :
    FileDownloader(accessManager, parent)
{
}

bool HttpDownloader::downloadFile(QUrl url, quint64 start, quint64 size, quint64 chunkSize)
{
    if(isDownloading()) {
        return false;
    }

    QByteArray data;


}

quint64 HttpDownloader::checkFileSize(QUrl url)
{
    QByteArray data;



}
