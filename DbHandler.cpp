#include "DbHandler.h"

DbHandler::DbHandler(QObject *parent) :
    QObject(parent)
{

}


DbHandler::~DbHandler()
{
    if (isOpened())
        database.close();
}


bool DbHandler::openDatabase(QString filepath)
{
    if (database.isOpen())
        return false;
    database = QSqlDatabase::addDatabase("QSQLITE", "DB");
    database.setDatabaseName(filepath);
    if (!database.open())
    {
        qDebug() << database.lastError();
        errorCode = OpenFailed;
        return false;
    }

    // sql语句
    QSqlQuery query(database);
    // 查询是否有工程信息
    if (!query.exec("select * from ProjectInfo") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoProjectInfo;
        return false;
    }
    // 查询是否有大图
    if (!query.exec("select * from bigImages") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoBigImages;
        return false;
    }

    // 查询是否有文本item
    if (!query.exec("select * from TextItem"))
    {
        query.exec("create table TextItem(x real, y real, content text)");
    }

    // 查询是否有缝宽item
    if (!query.exec("select * from WidthItem"))
    {
        query.exec("create table WidthItem(x1 real, y1 real, x2 real, y2 real)");
    }

    // 查询是否有角度item
    if (!query.exec("select * from AngleItem"))
    {
        query.exec("create table AngleItem(x1 real, y1 real, x2 real, y2 real, x3 real, y3 real)");
    }

    return true;
}

void DbHandler::closeDatabase()
{
    database.close();
}


bool DbHandler::isOpened()
{
    return database.isOpen();
}




DbHandler::PrjInfo DbHandler::getPrjInfo()
{
    PrjInfo prjInfo;
    QSqlQuery query(database);

    query.exec("select * from ProjectInfo");
    query.first();
    prjInfo.startHeight = query.value(1).toInt();


    query.exec("select * from bigImages");
    query.last();
    prjInfo.endHeight = query.value(0).toInt();

    return prjInfo;
}


DbHandler::BigImage DbHandler::getBigImage(quint16 index)
{
    QSqlQuery query(database);
    query.prepare("select * from bigImages where id > ? and id <= ?");
    query.bindValue(0, index * 10000);
    query.bindValue(1, (index + 1) * 10000);
    query.exec();
    query.first();

    BigImage bigImage;
    bigImage.start = index*10000;
    bigImage.end = query.value(0).toInt();
    QByteArray imgData = query.value(1).toByteArray();
    bigImage.pixmap.loadFromData(imgData);

    return bigImage;
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


