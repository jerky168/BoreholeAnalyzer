#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T14:38:43
#
#-------------------------------------------------

QT  += core gui sql opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoreholeAnalyzer
TEMPLATE = app

CONFIG += precompile_header debug_and_release

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = precompiled.h

SOURCES +=  main.cpp\
            mainwindow.cpp \
            GraphicsView.cpp \
            OpenGLWidget.cpp \
            items/GraphicsTextItem.cpp \
            DbHandler.cpp \
            GraphicsScene.cpp \
            TextDialog.cpp \
            items/GraphicsLineItem.cpp \
            GraphicsSettings.cpp \
            items/GraphicsAngleItem.cpp \
            items/GraphicsRectItem.cpp \
            items/GraphicsAnyshape.cpp \
            items/GraphicsOccurance.cpp \
    DefectWidget.cpp \
    ImageWidget.cpp \
    PrjInfoDialog.cpp \
    logo.cpp


HEADERS  += mainwindow.h \
            GraphicsView.h \
            OpenGLWidget.h \
            items/GraphicsTextItem.h \
            DbHandler.h \
            GraphicsScene.h \
            TextDialog.h \
            items/GraphicsLineItem.h \
            GraphicsSettings.h \
            items/GraphicsAngleItem.h \
            items/GraphicsRectItem.h \
            items/GraphicsAnyshape.h \
            items/GraphicsOccurance.h \
            items/type.h \
    DefectWidget.h \
    ImageWidget.h \
    PrjInfoDialog.h \
    logo.h

INCLUDEPATH += items

FORMS    += mainwindow.ui \
            imagewidget.ui \
            defectwidget.ui \
    PrjInfoDialog.ui

RESOURCES += \
    res.qrc

win32 {
    QT += axcontainer

    HEADERS += 3rd/qword.h  3rd/qexcel.h
    SOURCES += 3rd/qword.cpp 3rd/qexcel.cpp
    INCLUDEPATH += 3rd
}

DISTFILES += \
    doc/README.MD
