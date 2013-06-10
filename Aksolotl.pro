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

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    repositoryparser.cpp \
    filedownloader.cpp \
    ftpdownloader.cpp \
    httpdownloader.cpp \
    metadatafile.cpp \
    testmetadatafile.cpp

HEADERS  += mainwindow.h \
    repositoryparser.h \
    filedownloader.h \
    ftpdownloader.h \
    httpdownloader.h \
    metadatafile.h \
    testmetadatafile.h

FORMS    += mainwindow.ui
