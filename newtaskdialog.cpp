#include "newtaskdialog.h"
#include "ui_newtaskdialog.h"

NewTaskDialog::NewTaskDialog(Task *task, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTaskDialog)
{
    ui->setupUi(this);

    this->task = task;

    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(acceptedButton()));
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(rejectedButton()));
    connect(ui->selectSaveFileButton, SIGNAL(clicked()),
            this, SLOT(openFile()));

    ui->filename->setText(task->metadataFile()->getFilename());
    ui->filesize->setText(Helpers::humanReadableSize(task->metadataFile()->getFilesize()));
    ui->description->setPlainText(task->metadataFile()->getDescription());
    ui->mirrors->setText(QString::number(task->metadataFile()->getMirrorList().size()));
}

NewTaskDialog::~NewTaskDialog()
{
    delete ui;
}

void NewTaskDialog::rejectedButton()
{
    emit rejected();
}


void NewTaskDialog::acceptedButton()
{
    QString filePath = ui->saveFilePath->text();
    if(filePath == "") {
        return;
    }
    task->setOrigFileLocation(filePath);
    emit taskReady(task);
    close();
}

void NewTaskDialog::openFile()
{
    ui->saveFilePath->setText(QFileDialog::getSaveFileName(this, "Save file location", task->metadataFile()->getFilename()));
}
