#include "dbhandler.h"

DbHandler::DbHandler(QObject *parent) :
    QObject(parent)
{

}

DbHandler::DbHandler(QString filename, QObject *parent) :
    QObject(parent)
{
    openDatabase(filename);
}


DbHandler::~DbHandler()
{
    database.close();
}




void DbHandler::openDatabase(QString filepath)
{
    qDebug() << filepath;
    if (database.isOpen())
        return;
    database = QSqlDatabase::addDatabase("QSQLITE", "DB");
    database.setDatabaseName(filepath);
    if (!database.open())
    {
        qDebug() << database.lastError();
        errorCode = OpenFailed;
        return;
    }

    QSqlQuery query(database);

    if (!query.exec("select * from ProjectInfo") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoProjectInfo;
        return;
    }

    if (!query.exec("select * from bigImages") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoBigImages;
        return;
    }
}

DbHandler::PrjInfo DbHandler::getPrjInfo()
{
    PrjInfo prjInfo;
    QSqlQuery query(database);

    query.exec("select * from ProjectInfo");
    query.first();
    prjInfo.startHeight = query.value(1).toString();


    query.exec("select * from bigImages");
    query.last();
    prjInfo.endHeight = query.value(1).toString();




    return prjInfo;
}


QPixmap DbHandler::getBigImage(quint16 index)
{
    QSqlQuery query(database);
    query.prepare("select * from bigImages where id > ? and id <= ?");
    query.bindValue(0, index * 10000);
    query.bindValue(1, (index + 1) * 10000);
    query.exec();
    query.first();

    quint32 depth = query.value(0).toInt();
    QByteArray imgData = query.value(1).toByteArray();

    QPixmap pixmap;
    pixmap.loadFromData(imgData);
    return pixmap;
}


QVector<QPixmap> DbHandler::getSmallImage(quint32 start, quint32 end)
{
    QSqlQuery query(database);
    query.prepare("select * from bigImages where id > ? and id <= ?");
    query.bindValue(0, start);
    query.bindValue(1, end);
    query.exec();

    while (!query.next())
    {

    }
    return QVector<QPixmap>();
}

