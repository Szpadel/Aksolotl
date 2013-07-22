#include "editmetadatafilewindow.h"
#include "ui_editmetadatafilewindow.h"

EditMetadataFileWindow::EditMetadataFileWindow(MetadataFile *metafile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditMetadataFileWindow)
{
    ui->setupUi(this);

    if(metafile == NULL) {
        metafile = new MetadataFile(this);
    }
    
    metadataFile = metafile;
    if(! metadataFile->isLoaded()) {
        creatingNew = true;
    }else{
        creatingNew = false;
        loadFormFromMetafile();
        ui->openfile->hide();
        ui->chunklvl->setReadOnly(true);
    }

    connect(ui->openfile, SIGNAL(clicked()),
            this, SLOT(openFile()));
    connect(ui->chunklvl, SIGNAL(valueChanged(int)),
            this, SLOT(onChunkSizeChanged()));
    connect(ui->filesize, SIGNAL(textChanged(QString)),
           this, SLOT(updateHumanFilesize()));
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(save()));
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(close()));
    connect(ui->actionClose, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->add, SIGNAL(clicked()),
            this, SLOT(addMirror()));
    connect(ui->remove, SIGNAL(clicked()),
            this, SLOT(removeMirror()));

    ui->filesize->setValidator(new QRegExpValidator(Helpers::digitalOnlyRegExp()));
    onChunkSizeChanged();
    updateHumanFilesize();
}

EditMetadataFileWindow::~EditMetadataFileWindow()
{
    delete ui;
}

void EditMetadataFileWindow::loadFormFromMetafile()
{
    ui->filename->setText(metadataFile->getFilename());
    ui->filesize->setText(QString::number(metadataFile->getFilesize()));
    ui->description->setPlainText(metadataFile->getDescription());
    ui->chunklvl->setValue(metadataFile->getChunkSizeLevel());
}

void EditMetadataFileWindow::openFile()
{
    origFile.setFileName(QFileDialog::getOpenFileName(this, "Select file"));
    ui->filename->setText(QFileInfo(origFile.fileName()).fileName());
    ui->filesize->setText(QString::number(origFile.size()));
}

void EditMetadataFileWindow::onChunkSizeChanged()
{
    int chunkLvl = ui->chunklvl->value();
    int chunkSize = pow(2, chunkLvl);
    ui->chunksize->setText(Helpers::humanReadableSize(chunkSize*1024, 0));
}

void EditMetadataFileWindow::updateHumanFilesize()
{
    quint64 filesize = ui->filesize->text().toULongLong();
    ui->humanFilesize->setText(Helpers::humanReadableSize(filesize));
}

void EditMetadataFileWindow::save(bool saveAs)
{
    QString saveFilename = "";
    if(creatingNew || saveAs) {
        saveFilename = QFileDialog::getSaveFileName(this, "Save Metadata file", "", "*.axl");
        if(saveFilename == "") {
            return;
        }
    }

    metadataFile->setFilename(ui->filename->text());
    metadataFile->setFilesize(ui->filesize->text().toULongLong());
    metadataFile->setDescription(ui->description->toPlainText());

    if(creatingNew) {
        origFile.open(QIODevice::ReadOnly);
        qDebug() << origFile.fileName();
        if(! origFile.isReadable()) {
            QMessageBox::information(this, "error", "Can't open target file");
            return;
        }

        metadataFile->setChunkSizeLevel(ui->chunklvl->value());


        for(quint64 a = 0; a < ceil((double)metadataFile->getFilesize()/metadataFile->getChunkSize()); a++) {
            origFile.seek(a*metadataFile->getChunkSize());
            QByteArray ch;
            ch = origFile.read(metadataFile->getChunkSize());
            metadataFile->addChunk(qChecksum(ch, ch.size()));
        }
    }

    metadataFile->save(saveFilename);
    this->close();
}


void EditMetadataFileWindow::removeMirror()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void EditMetadataFileWindow::addMirror()
{
    ui->tableWidget->insertRow(0);
    ui->tableWidget->item(0, 0)
            ->setText(
                QInputDialog::getText(this, "Mirror URL", "URL:"));
}
