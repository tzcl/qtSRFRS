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
    settings.cpp \
    flashcard.cpp \
    deck.cpp \
    deckmanager.cpp \
    flashcardcreator.cpp \
    deckcreator.cpp \
    deckeditor.cpp \
    deckrenamer.cpp \
    flashcardeditor.cpp \
    flashcardmanager.cpp \
    flashcardpreviewer.cpp \
    deckreviewer.cpp \
    loginwindow.cpp

HEADERS += \
        mainwindow.h \
    accountmanager.h \
    settings.h \
    flashcard.h \
    deck.h \
    deckmanager.h \
    flashcardcreator.h \
    deckcreator.h \
    deckeditor.h \
    deckrenamer.h \
    flashcardeditor.h \
    flashcardmanager.h \
    flashcardpreviewer.h \
    deckreviewer.h \
    loginwindow.h \
    complexalgorithm.h

FORMS += \
        mainwindow.ui \
    settings.ui \
    flashcardcreator.ui \
    deckcreator.ui \
    deckeditor.ui \
    deckrenamer.ui \
    flashcardeditor.ui \
    flashcardpreviewer.ui \
    deckreviewer.ui \
    loginwindow.ui

RESOURCES += \
    res.qrc
