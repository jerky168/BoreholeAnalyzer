#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T14:38:43
#
#-------------------------------------------------

QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT  += opengl sql


TARGET = BoreholeAnalyzer
TEMPLATE = app

CONFIG += precompile_header debug_and_release

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = precompiled.h

TRANSLATIONS = BoreholeAnalyzer_zh_CN.ts

SOURCES +=  main.cpp\
            mainwindow.cpp \          
            DbHandler.cpp \           
            GraphicsSettings.cpp \
            DefectWidget.cpp \
            ImageWidget.cpp \
            PrjInfoDialog.cpp \
            ToolBar.cpp \
            AboutDialog.cpp \
            ManualDialog.cpp \
            ContactDialog.cpp \
            RollWidget.cpp \
            TextDialog.cpp \
            GraphicsScene.cpp \ 
            GraphicsView.cpp \
            items/GraphicsAngleItem.cpp \
            items/GraphicsRectItem.cpp \
            items/GraphicsAnyshape.cpp \
            items/GraphicsOccurance.cpp \
            items/GraphicsLineItem.cpp \
            items/GraphicsTextItem.cpp



HEADERS  += mainwindow.h \          
            DbHandler.h \          
            GraphicsSettings.h \
            DefectWidget.h \
            ImageWidget.h \
            PrjInfoDialog.h \
            ToolBar.h \
            AboutDialog.h \
            ManualDialog.h \
            ContactDialog.h \
            RollWidget.h \
            TextDialog.h \
            GraphicsScene.h \
            GraphicsView.h \
            items/GraphicsAngleItem.h \
            items/GraphicsRectItem.h \
            items/GraphicsAnyshape.h \
            items/GraphicsOccurance.h \
            items/GraphicsTextItem.h \
            items/GraphicsLineItem.h \
            items/type.h


INCLUDEPATH += items

FORMS    += mainwindow.ui \
            PrjInfoDialog.ui \
            DefectWidget.ui \
            ImageWidget.ui \
            AboutDialog.ui \
            ManualDialog.ui \
            ContactDialog.ui




RESOURCES += \
    res.qrc

win32 {
    QT += axcontainer

    HEADERS += 3rd/QWord.h  3rd/QExcel.h
    SOURCES += 3rd/QWord.cpp 3rd/QExcel.cpp
    INCLUDEPATH += 3rd

    LIBS += -lopengl32 -lglu32

}

DISTFILES += \
    doc/README.MD

unix{
    LIBS += -lglut -lGLU
}


