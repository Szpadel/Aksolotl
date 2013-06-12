#ifndef EDITMETADATAFILEWINDOW_H
#define EDITMETADATAFILEWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QRegExp>
#include <QFileInfo>
#include <QAbstractButton>
#include <QDebug>

#include <metadatafile.h>
#include <helpers.h>

namespace Ui {
class EditMetadataFileWindow;
}

class EditMetadataFileWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    EditMetadataFileWindow(MetadataFile* metafile = NULL, QWidget *parent = 0);
    ~EditMetadataFileWindow();
    
private:
    void loadFormFromMetafile();

    Ui::EditMetadataFileWindow *ui;
    MetadataFile* metadataFile;
    QFile origFile;
    bool creatingNew;

private slots:
    void openFile();
    void onChunkSizeChanged();
    void updateHumanFilesize();
    void save(bool saveAs = false);
};

#endif // EDITMETADATAFILEWINDOW_H
