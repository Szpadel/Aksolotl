#include "task.h"

Task::Task(DownloadManager *dm, QObject *parent) :
    QObject(parent)
{
    downloadManager = dm;
}
