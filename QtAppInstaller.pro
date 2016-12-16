#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T23:15:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAppInstaller
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    applist.cpp \
    installdialog.cpp

HEADERS  += mainwindow.h \
    applist.h \
    installdialog.h

FORMS    += mainwindow.ui \
    installdialog.ui

RESOURCES += \
    langs.qrc
