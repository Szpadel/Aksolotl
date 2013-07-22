#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionCreate_metadata_file, SIGNAL(triggered()),
            this, SLOT(openNewMetadataFileWindow()));
    connect(ui->actionOpen_metadata_file, SIGNAL(triggered()),
            this, SLOT(loadMetadataFile()));
    connect(ui->tasksTable, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(selectTask(QTreeWidgetItem*)));
    connect(ui->actionStart_Task, SIGNAL(triggered()),
            this, SLOT(startTask()));
    dm = new DownloadManager(this);
    ui->tabWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(editMetadataFileWindow != NULL) {
        delete editMetadataFileWindow;
    }
}

void MainWindow::openNewMetadataFileWindow()
{
    openEditMetadataWindow(NULL);
}

void MainWindow::openEditMetadataWindow(MetadataFile *mf)
{
    if(editMetadataFileWindow != NULL) {
        editMetadataFileWindow->close();
        delete editMetadataFileWindow;
        editMetadataFileWindow = NULL;
    }
    editMetadataFileWindow = new EditMetadataFileWindow(mf, this);
    editMetadataFileWindow->setParent(this);
    editMetadataFileWindow->show();

}

void MainWindow::loadMetadataFile()
{
    MetadataFile *meta = new MetadataFile();
    meta->open(QFileDialog::getOpenFileName(this, "Plik metadanych", "", "*.axl"));
    if(meta->isLoaded()) {
        Task *newTask = new Task(dm, meta);
        NewTaskDialog dialog(newTask, this);
        connect(&dialog, SIGNAL(taskReady(Task*)),
                this, SLOT(addTask(Task*)));
        dialog.exec();
    } else {
        QMessageBox::information(this, "Open Error", "Can't open file");
    }
}

void MainWindow::addTask(Task* task)
{
    tasks.append(task);
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, task->getOrigFileLocation()); // filename
    //item->setText(1, task->getTaskStatusString()); // TODO: implementacja, status
    item->setText(2, "0%"); // progress
    item->setText(3, Helpers::humanReadableSize(task->metadataFile()->getFilesize())); // filesize
    item->setText(4, QString::number(task->metadataFile()->getMirrorList().size())); // mirrors
    item->setText(5, "unknown"); // speed
    item->setText(6, "unknown"); // eta
    ui->tasksTable->addTopLevelItem(item);
    connect(task, SIGNAL(taskStatusChanged(Task*)),
            this, SLOT(refreshTaskList()));
    connect(task, SIGNAL(progressChanged(int)),
           this, SLOT(refreshTaskList()));
}

void MainWindow::refreshTaskList()
{
    QTreeWidgetItem *item = ui->tasksTable->topLevelItem(0);
    Q_FOREACH(Task* task, tasks)
    {
        item->setText(0, task->getOrigFileLocation()); // filename
        //item->setText(1, task->getTaskStatusString()); // TODO: implementacja, status
        item->setText(2, QString::number(task->progress()) + "%"); // progress
        item->setText(3, Helpers::humanReadableSize(task->metadataFile()->getFilesize())); // filesize
        item->setText(4, QString::number(task->metadataFile()->getMirrorList().size())); // mirrors
        item->setText(5, "unknown"); // speed
        item->setText(6, "unknown"); // eta

        if(item == selected) {
            selectedTask = task;
            ui->taskFilename->setText(task->metadataFile()->getFilename());
            ui->taskFilesize->setText(Helpers::humanReadableSize(task->metadataFile()->getFilesize()));
            ui->taskDownloaded->setText(Helpers::humanReadableSize(
                                            task->chunksOk() * task->metadataFile()->getChunkSize()
                                            ));
            ui->taskTotalChunks->setText(QString::number(task->metadataFile()->getChecksumList().size()));
            ui->taskOkChunks->setText(QString::number(task->chunksOk()));
            ui->taskCorruptedChunks->setText(QString::number(task->chunksCorrupted()));
            ui->taskMissingChunks->setText(QString::number(task->chunksMissing()));
            ui->taskFileLocation->setText(task->getOrigFileLocation());
            ui->taskChunkSize->setText(Helpers::humanReadableSize(task->metadataFile()->getChunkSize()));
            ui->taskDescription->setPlainText(task->metadataFile()->getDescription());
            ui->taskProgress->setValue(task->progress());

        }
        item = ui->tasksTable->itemBelow(item);
    }
}

void MainWindow::selectTask(QTreeWidgetItem *item)
{
    ui->tabWidget->show();
    this->selected = item;
    refreshTaskList();
}

void MainWindow::startTask()
{
    /*QTimer t;
    t.setInterval(0);
    t.setSingleShot(true);
    connect(&t, SIGNAL(timeout()),
            selectedTask, SLOT(start()));
    t.start();*/
    selectedTask->start();
}
