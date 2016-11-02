#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T14:38:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoreholeAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageframe.cpp \
    coreframe.cpp

HEADERS  += mainwindow.h \
    imageframe.h \
    coreframe.h

FORMS    += mainwindow.ui \
    imageframe.ui \
    coreframe.ui

RESOURCES += res.qrc
