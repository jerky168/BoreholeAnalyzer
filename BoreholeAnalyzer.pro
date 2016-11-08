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

SOURCES +=  main.cpp\
            mainwindow.cpp \
            imagewidget.cpp \
            defectwidget.cpp \
    dbhandler.cpp \
    dbbigimages.cpp


HEADERS  += mainwindow.h \
            imagewidget.h \
            defectwidget.h \
    dbhandler.h \
    precompiled.h \
    dbbigimages.h


FORMS    += mainwindow.ui \
            imagewidget.ui \
            defectwidget.ui

RESOURCES += \
    res.qrc

win32 {
#    QT += axcontainer

#    HEADERS += qword.h  qexcel.h
#    SOURCES += qword.cpp qexcel.cpp

    INCLUDEPATH += $$(QXORM_DIR)\include \
                   $$(BOOST_INCLUDE)
    CONFIG(release, debug|release): LIBS += -L$$(QXORM_DIR)/lib -lQxOrm
    else:CONFIG(debug, debug|release): LIBS += -L$$(QXORM_DIR)/lib -lQxOrmd

}

DISTFILES += \
    doc/README.MD
