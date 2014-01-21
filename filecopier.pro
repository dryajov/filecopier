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
    copierworker.cpp \
    filebrowser.cpp \
    filecopywidget.cpp \
    engines/copyenginedefault.cpp

HEADERS  += mainwindow.h \
    copierworker.h \
    filebrowser.h \
    filecopywidget.h \
    icopyengine.h \
    engines/copyengineabstract.h \
    engines/copyenginedefault.h

FORMS    += mainwindow.ui \
    filebrowser.ui \
    filecopywidget.ui
