#include "httpdownloader.h"

HttpDownloader::HttpDownloader(QNetworkAccessManager *accessManager, QObject *parent) :
    FileDownloader(accessManager, parent)
{
}

bool HttpDownloader::downloadFile(QUrl url, quint64 start, quint64 size, int chunkSize)
{
    if(chunkSize == 0)
    {
        chunkSize = -1;
    }

    if(isDownloading()) {
        return false;
    }

    QNetworkRequest request;
    QNetworkReply *reply;
    QByteArray data;
    QString range;

    range = "bytes=" + QString::number(start) + "-";

    if(size>0){
        range += QString::number(start+size);
    }

    request.setUrl(url);
    request.setRawHeader(QString("Range").toAscii(), range);

    reply = this->accessManager->get(request);

    QVariant variant;
    variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if( variant.toInt() != 206){
        this->errorStatus = FileDownloader::ErrorType::PARTIAL_NOT_SUPPORTED;
        emit this->error(this->errorStatus, "partial content not supported by the server");
        return false;
    }

    bool isEndOfFile = false;
    while(reply->waitForReadyRead(10000) && ! isEndOfFile)
    {
        data.append(reply->readAll());
        while(! isEndOfFile && ( ( data.size() >= chunkSize && chunkSize > 0) || reply->isFinished() ) ){
            QByteArray chunk(data.mid(0,chunkSize));
            emit chunkDownloaded(chunk);
            if(reply->isFinished()){
                isEndOfFile = true;
                break;
            }
            data.remove(0, chunkSize);
        }
    }
    return true;

}

quint64 HttpDownloader::checkFileSize(QUrl url)
{
    QNetworkRequest request;
    QNetworkReply *reply;

    request.setUrl(url);
    reply = this->accessManager->sendCustomRequest(request,QString("HEAD").toAscii());

    QVariant variant;
    variant = reply->header(QNetworkRequest::ContentLengthHeader);
    if( ! variant.isValid()){
        return 0;
    }
    return variant.toULongLong();
}

