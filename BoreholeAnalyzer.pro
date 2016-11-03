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
    mainwidget.cpp \
    imagewidget.cpp \
    defectwidget.cpp

HEADERS  += mainwindow.h \
    mainwidget.h \
    imagewidget.h \
    defectwidget.h

FORMS    += mainwindow.ui \
    mainwidget.ui \
    imagewidget.ui \
    defectwidget.ui

RESOURCES += \
    res.qrc


