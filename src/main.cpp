#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/BoreholeAnalyzer_") + locale);
    a.installTranslator(&translator);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}

