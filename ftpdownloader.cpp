#include "ftpdownloader.h"

FtpDownloader::FtpDownloader(QNetworkAccessManager *accessManager, QObject *parent) :
    FileDownloader(accessManager, parent)
{
}

bool FtpDownloader::downloadFile(QUrl url, quint64 start, quint64 size, int chunkSize)
{
    if(chunkSize == 0){
        chunkSize = -1;
    }

    if(isDownloading()){
        return false;
    }

    isDownloading = true;



}

quint64 FtpDownloader::checkFileSize(QUrl url)
{

}

bool FtpDownloader::isDownloading()
{
    return isDownloading;
}
