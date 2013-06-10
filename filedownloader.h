#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // FILEDOWNLOADER_H
