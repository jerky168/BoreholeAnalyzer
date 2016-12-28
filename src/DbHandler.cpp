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

    QSqlQuery query(database);

    if (!query.exec("select * from ProjectInfo") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoProjectInfo;
        return false;
    }

    if (!query.exec("select * from bigImages") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoBigImages;
        return false;
    }


    if (!query.exec("select * from items"))
    {
        query.exec("CREATE TABLE items (uuid TEXT PRIMARY KEY NOT NULL, number INT NOT NULL, type INT NOT NULL, data TEXT);");
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

    query.exec("select direction, diameter, startHeight, projectName, date, orificeNumber from ProjectInfo");
    query.first();
    if (query.value(0).toString().contains("down"))
        prjInfo.isUp2Down = true;
    else
        prjInfo.isUp2Down = false;
    prjInfo.diameter = query.value(1).toDouble() / 1000;
    prjInfo.startHeight = query.value(2).toDouble() / 10000;
    prjInfo.projectName = query.value(3).toString();
    prjInfo.projectTime = query.value(4).toString();
    prjInfo.orificeNumber = query.value(5).toString();

    query.exec("select * from bigImages");
    query.last();
    prjInfo.endHeight = query.value(0).toDouble() / 10000;

    return prjInfo;
}


void DbHandler::setPrjInfo(PrjInfo prjInfo)
{
    QSqlQuery query(database);
    //query.prepare("INSERT INTO items (uuid, number, type, data) VALUES (:uuid, :number, :type, :data)")
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
    bigImage.start = index;
    bigImage.end = (qreal)query.value(0).toInt() / 10000.0;
    QByteArray imgData = query.value(1).toByteArray();
    bigImage.pixmap.loadFromData(imgData);

    return bigImage;
}



void DbHandler::saveItem(QUuid uuid, quint16 index, quint8 type, QString dataStr)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO items (uuid, number, type, data) VALUES (:uuid, :number, :type, :data)");
    query.bindValue(":uuid", uuid.toString());
    query.bindValue(":number", index);
    query.bindValue(":type", type);
    query.bindValue(":data", dataStr);
    query.exec();
}


DbHandler::IndexData DbHandler::getIndexData(quint16 index)
{
    IndexData indexData;
    indexData.image = getBigImage(index);

    QSqlQuery query(database);
    query.prepare("SELECT uuid, type, data FROM items WHERE number = :number");
    query.bindValue(":number", index);
    query.exec();

    ItemData itemData;
    while(query.next())
    {
        itemData.uuid = QUuid(query.value("uuid").toString());
        itemData.type = query.value("type").toInt();
        itemData.dataStr = query.value("data").toString();
        indexData.itemDatas.append(itemData);
    }

    return indexData;
}





