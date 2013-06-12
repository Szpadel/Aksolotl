#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>

#include <editmetadatafilewindow.h>
#include <metadatafile.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    void openEditMetadataWindow(MetadataFile* mf);

    Ui::MainWindow *ui;
    EditMetadataFileWindow *editMetadataFileWindow = NULL;
    QList<MetadataFile*> openedMetadataFiles;

public slots:
    void openNewMetadataFileWindow();
    void loadMetadataFile();
};

#endif // MAINWINDOW_H
