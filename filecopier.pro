#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T00:07:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filecopier
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    copyfilewidget.cpp

HEADERS  += mainwindow.h \
    copyfilewidget.h

FORMS    += mainwindow.ui \
    copyfilewidget.ui
