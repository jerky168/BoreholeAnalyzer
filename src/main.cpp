#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName(App_Name_EN);
    QApplication::setApplicationVersion(App_Version);
    QApplication::setOrganizationName(Company_Name_EN);
    QApplication::setOrganizationDomain(Company_Url);
    QApplication::setWindowIcon(QIcon(":/res/images/IDT.ico"));

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/BoreholeAnalyzer_") + locale);
    a.installTranslator(&translator);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}

