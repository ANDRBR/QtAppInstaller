#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T23:15:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAppInstaller
TEMPLATE = app

VERSION = 1.2.0.1
DEFINES += APP_VERSION='"\\\"v$$VERSION\\\""'

SOURCES += main.cpp\
        mainwindow.cpp \
    applist.cpp \
    installdialog.cpp \
    editcsvlist.cpp

HEADERS  += mainwindow.h \
    applist.h \
    installdialog.h \
    editcsvlist.h

FORMS    += mainwindow.ui \
    installdialog.ui \
    editcsvlist.ui

RESOURCES += \
    langs.qrc
