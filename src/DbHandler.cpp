#include "DbHandler.h"

DbHandler::DbHandler(QObject *parent) :
    QObject(parent)
{
    projectInfoFields.insert("direction", "TEXT");
    projectInfoFields.insert("diameter", "INTEGER");
    projectInfoFields.insert("startDepth", "INTEGER");
    projectInfoFields.insert("endDepth", "INTEGER");
    projectInfoFields.insert("orificeNumber", "TEXT");
    projectInfoFields.insert("projectName", "TEXT");
    projectInfoFields.insert("projectTime", "TEXT");
    projectInfoFields.insert("projectSite", "TEXT");
    projectInfoFields.insert("projectPerson", "TEXT");
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

    // 检查是否有ProjectInfo表且里面是否有数据
    if (!query.exec("select * from ProjectInfo") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoProjectInfo;
        return false;
    }
    // 检查表中是否有所有的字段 没有则添加
    else if (!query.exec("select projectSite from ProjectInfo"))
    {
        query.exec("alter table ProjectInfo add projectSite TEXT");
    }


    if (!query.exec("select * from bigImages") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoBigImages;
        return false;
    }


    if (!query.exec("select * from items"))
    {
        query.exec("CREATE TABLE items (uuid TEXT PRIMARY KEY NOT NULL, number INT NOT NULL, type INT NOT NULL, data TEXT, remark TEXT);");
    }
    else if (!query.exec("select remark from bigImages"))
    {
        query.exec("alter table items add remark TEXT");
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
    if (query.value("direction").toString().contains("down"))
        prjInfo.isUp2Down = true;
    else
        prjInfo.isUp2Down = false;
    prjInfo.diameter = query.value("diameter").toInt() / 10000.0;
    prjInfo.startHeight = query.value("startHeight").toDouble() / 10000;
    prjInfo.projectName = query.value("projectName").toString();
    prjInfo.projectTime = query.value("date").toString();
    prjInfo.orificeNumber = query.value("orificeNumber").toString();
    prjInfo.projectSite = query.value("projectSite").toString();

    query.exec("select * from bigImages");
    query.last();
    prjInfo.endHeight = query.value(0).toDouble() / 10000;

    return prjInfo;
}



void DbHandler::setPrjInfo(PrjInfo prjInfo)
{
    if (!isOpened())
        return;

    QSqlQuery query(database);
    query.exec("DELETE FROM ProjectInfo");
    query.prepare("INSERT INTO ProjectInfo (diameter, startHeight, projectName, date, orificeNumber, projectSite) "
                  "VALUES (:diameter, :startHeight, :projectName, :date, :orificeNumber, :projectSite)");
    query.bindValue(":diameter", prjInfo.diameter);
    query.bindValue(":startHeight", prjInfo.startHeight * 10000);
    query.bindValue(":projectName", prjInfo.projectName);
    query.bindValue(":date", prjInfo.projectTime);
    query.bindValue(":orificeNumber", prjInfo.orificeNumber);
    query.bindValue(":projectSite", prjInfo.projectSite);
    query.exec();
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

    query.exec("select diameter from ProjectInfo");
    query.first();
    bigImage.diameter = query.value("diameter").toInt() / 10000.0;

    return bigImage;
}

void DbHandler::setBigImage(qreal start, qreal end, QImage image)
{
    QSqlQuery query(database);
    query.prepare("UPDATE bigImages SET data = :data WHERE id = :id");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    query.bindValue(":data", ba);
    query.bindValue(":id", end * 10000);
    query.exec();
}



void DbHandler::saveItem(QUuid uuid, quint16 index, quint8 type, QString dataStr, QString remark)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO items (uuid, number, type, data, remark) VALUES (:uuid, :number, :type, :data, :remark)");
    query.bindValue(":uuid", uuid.toString());
    query.bindValue(":number", index);
    query.bindValue(":type", type);
    query.bindValue(":data", dataStr);
    query.bindValue(":remark", remark);
    query.exec();
}



void DbHandler::deleteItem(QUuid uuid)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM items WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid.toString());
    query.exec();
}


void DbHandler::updateItemremark(QUuid uuid, QString remark)
{
    QSqlQuery query(database);
    query.prepare("update items set remark = :remark where uuid = :uuid");
    query.bindValue(":remark", remark);
    query.bindValue(":uuid", uuid.toString());
    query.exec();
}


DbHandler::IndexData DbHandler::getIndexData(quint16 index)
{
    IndexData indexData;
    indexData.image = getBigImage(index);

    QSqlQuery query(database);
    query.prepare("SELECT * FROM items WHERE number = :number");
    query.bindValue(":number", index);
    query.exec();

    ItemData itemData;
    while(query.next())
    {
        itemData.uuid = QUuid(query.value("uuid").toString());
        itemData.type = query.value("type").toInt();
        itemData.dataStr = query.value("data").toString();
        itemData.remark = query.value("remark").toString();
        indexData.itemDatas.append(itemData);
    }

    return indexData;
}





