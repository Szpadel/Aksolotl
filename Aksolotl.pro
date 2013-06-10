#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T18:49:05
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aksolotl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    repositoryparser.cpp \
    filedownloader.cpp \
    ftpdownloader.cpp \
    httpdownloader.cpp

HEADERS  += mainwindow.h \
    repositoryparser.h \
    filedownloader.h \
    ftpdownloader.h \
    httpdownloader.h

FORMS    += mainwindow.ui
