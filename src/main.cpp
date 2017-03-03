#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用各属性
    QApplication::setApplicationName(App_Name_EN);
    QApplication::setApplicationVersion(App_Version);
    QApplication::setOrganizationName(Company_Name_EN);
    QApplication::setOrganizationDomain(Company_Url);
    QApplication::setWindowIcon(QIcon(":/res/images/IDT.ico"));

    // 添加翻译文件
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(":/BoreholeAnalyzer_") + locale);
    a.installTranslator(&translator);

    // 显示窗体
    MainWindow w;
    w.showMaximized();

    return a.exec();
}

