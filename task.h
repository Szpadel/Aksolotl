#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QFile>

#include <metadatafile.h>
#include <downloadmanager.h>

class Task : public QObject
{
    Q_OBJECT
public:
    enum TaskStatus {
        STOPPED,
        DOWNLOADING,
        CHECKING,
        ERROR
    };

    Task(DownloadManager *dm, QObject *parent = 0);
    MetadataFile* metadataFile();
    int progress();



protected:
    MetadataFile* metaFile;
    QFile origFile;
    DownloadManager* downloadManager;
    
signals:
    
public slots:
    
};

#endif // TASK_H
