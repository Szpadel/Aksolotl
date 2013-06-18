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
    dm = new DownloadManager(this);
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
        Task *newTask = new Task(dm, meta, this);
        NewTaskDialog dialog(newTask, this);
        dialog.exec();
    } else {
        QMessageBox::information(this, "Open Error", "Can't open file");
    }
}

void MainWindow::addTask(Task* task)
{
    tasks.append(task);
    QTreeWidgetItem item();
    item.setText(0, task->getOrigFileLocation()); // filename
    //item.setText(1, task->getTaskStatusString()); // TODO: implementacja, status
    item.setText(2, "0%"); // progress
    item.setText(3, Helpers::humanReadableSize(task->metadataFile()->getFilesize())); // filesize
    item.setText(4, QString::number(task->metadataFile()->getMirrorList().size())); // mirrors
    item.setText(5, "unknown"); // speed
    item.setText(6, "unknown"); // eta
    ui->tasksTable->addTopLevelItem(item);
    connect(task, SIGNAL(taskStatusChanged(Task*)),
            this, SLOT(refreshTaskList()));
    conect(task, SIGNAL(taskStatusChanged(Task*)),
           this, SLOT(refreshTaskList()));
}

void MainWindow::refreshTaskList()
{
    QTreeWidgetItem *item = ui->tasksTable->header();
    Q_FOREACH(Task* task, tasks)
    {
        item->setText(0, task->getOrigFileLocation()); // filename
        //item->setText(1, task->getTaskStatusString()); // TODO: implementacja, status
        item->setText(2, "0%"); // progress
        item->setText(3, Helpers::humanReadableSize(task->metadataFile()->getFilesize())); // filesize
        item->setText(4, QString::number(task->metadataFile()->getMirrorList().size())); // mirrors
        item->setText(5, "unknown"); // speed
        item->setText(6, "unknown"); // eta
        item = ui->tasksTable->BelowItem(item);
    }
}
