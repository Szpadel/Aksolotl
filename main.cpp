#include "mainwindow.h"
#include "repositoryparser.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    RepositoryParser lol;
    qDebug()<<lol.parseRepositories("ftp://śćś©śóśðąśðó\nasdad\nhtt://www.chujchuj.pl");
    return a.exec();
}
