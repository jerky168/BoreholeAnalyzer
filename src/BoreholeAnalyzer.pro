#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T14:38:43
#
#-------------------------------------------------

QT  += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoreholeAnalyzer
TEMPLATE = app

CONFIG += precompile_header debug_and_release


RC_ICONS = res/images/IDT.ico


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
            RollWidget.cpp \
            TextDialog.cpp \
            GraphicsScene.cpp \ 
            GraphicsView.cpp \
            items/GraphicsAngleItem.cpp \
            items/GraphicsRectItem.cpp \
            items/GraphicsAnyshape.cpp \
            items/GraphicsOccurance.cpp \
            items/GraphicsLineItem.cpp \
            items/GraphicsTextItem.cpp \
    ShiftDialog.cpp



HEADERS  += mainwindow.h \          
            DbHandler.h \          
            GraphicsSettings.h \
            DefectWidget.h \
            ImageWidget.h \
            PrjInfoDialog.h \
            ToolBar.h \
            AboutDialog.h \
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
            items/type.h \
            app.h \
    ShiftDialog.h


INCLUDEPATH += items

FORMS    += mainwindow.ui \
            PrjInfoDialog.ui \
            DefectWidget.ui \
            ImageWidget.ui \
            AboutDialog.ui \
    ShiftDialog.ui


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
    ../doc/README.MD

unix{
    LIBS += -lglut -lGLU
}


