#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T14:38:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoreholeAnalyzer
TEMPLATE = app

CONFIG += precompile_header debug_and_release

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = stable.h

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

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

win32 {
    OBJECTS_DIR = $$DESTDIR/obj
    MOC_DIR = $$DESTDIR/moc
    RCC_DIR = $$DESTDIR/qrc
    UI_DIR = $$DESTDIR/ui
} else {
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}
