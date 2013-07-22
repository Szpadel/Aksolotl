#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTimer>

#include <editmetadatafilewindow.h>
#include <metadatafile.h>
#include <newtaskdialog.h>
#include <downloadmanager.h>

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
    QList<Task*> tasks;

    DownloadManager *dm;
    QTreeWidgetItem* selected;
    Task* selectedTask;

public slots:
    void openNewMetadataFileWindow();
    void loadMetadataFile();
    void addTask(Task* task);
    void refreshTaskList();
    void selectTask(QTreeWidgetItem * item);
    void startTask();
};

#endif // MAINWINDOW_H
