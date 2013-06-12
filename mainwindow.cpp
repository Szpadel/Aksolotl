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
        openedMetadataFiles.append(meta);
    } else {
        QMessageBox::information(this, "Open Error", "Can't open file");
    }
}
