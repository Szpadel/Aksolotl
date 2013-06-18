#ifndef NEWTASKDIALOG_H
#define NEWTASKDIALOG_H

#include <QDialog>
#include <QFileDialog>

#include <task.h>
#include <helpers.h>

namespace Ui {
class NewTaskDialog;
}

class NewTaskDialog : public QDialog
{
    Q_OBJECT
    
public:
    NewTaskDialog(Task* task, QWidget *parent = 0);
    ~NewTaskDialog();
    
private:
    Task *task;
    Ui::NewTaskDialog *ui;

private slots:
    void acceptedButton();
    void rejectedButton();
    void openFile();

signals:
    void taskReady(Task * task);
    void rejected();
};

#endif // NEWTASKDIALOG_H
