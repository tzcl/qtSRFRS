#-------------------------------------------------
#
# Project created by QtCreator 2017-07-18T12:52:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtSRFRS
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    accountmanager.cpp \
    settings.cpp

HEADERS += \
        mainwindow.h \
    accountmanager.h \
    settings.h

FORMS += \
        mainwindow.ui \
    settings.ui

RESOURCES += \
    res.qrc