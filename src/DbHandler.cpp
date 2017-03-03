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

    if (!query.exec("select * from BigImages") || !query.first())
    {
        qDebug() << query.lastError().text();
        errorCode = NoBigImages;
        return false;
    }

    if (!query.exec("select * from items"))
    {
        query.exec("CREATE TABLE items (uuid TEXT PRIMARY KEY NOT NULL, number INT NOT NULL, depth INT NOT NULL, type INT NOT NULL, data TEXT, remark TEXT);");
    }
    else if (!query.exec("select remark from BigImages"))
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
    prjInfo.diameter = query.value("diameter").toDouble();
    prjInfo.startHeight = query.value("startHeight").toDouble() / 10000.0;
    prjInfo.projectName = query.value("projectName").toString();
    prjInfo.projectTime = query.value("date").toString();
    prjInfo.orificeNumber = query.value("orificeNumber").toString();
    prjInfo.projectSite = query.value("projectSite").toString();

    query.exec("select * from BigImages");
    query.last();
    prjInfo.endHeight = query.value(0).toDouble() / 10000.0;

    return prjInfo;
}



void DbHandler::setPrjInfo(PrjInfo prjInfo)
{
    if (!isOpened())
        return;

    QSqlQuery query(database);

    query.prepare("UPDATE ProjectInfo "
                  "SET startHeight = :startHeight, diameter = :diameter, projectName = :projectName, "
                  "orificeNumber = :orificeNumber, date = :date, projectSite = :projectSite");

    query.bindValue(":startHeight", prjInfo.startHeight * 10000);
    query.bindValue(":diameter", prjInfo.diameter / 1000.0);
    query.bindValue(":projectName", prjInfo.projectName);
    query.bindValue(":orificeNumber", prjInfo.orificeNumber);
    query.bindValue(":date", prjInfo.projectTime);
    query.bindValue(":projectSite", prjInfo.projectSite);
    if (!query.exec())
        qDebug() << query.lastError().text();


}


DbHandler::BigImage DbHandler::getBigImage(qint32 index)
{
    QSqlQuery query(database);
    query.prepare("select * from BigImages where id > ? and id <= ?");
    query.bindValue(0, index * 10000);
    query.bindValue(1, (index + 1) * 10000);
    query.exec();
    query.first();

    BigImage bigImage;
    bigImage.start = index;
    bigImage.end = query.value(0).toInt() / 10 / 1000.0;
    QByteArray imgData = query.value(1).toByteArray();
    bigImage.pixmap.loadFromData(imgData);

    query.exec("select diameter from ProjectInfo");
    query.first();
    bigImage.diameter = query.value("diameter").toDouble();

    return bigImage;
}

void DbHandler::setBigImage(qreal start, qreal end, QImage image)
{
    QSqlQuery query(database);
    query.prepare("UPDATE BigImages SET data = :data WHERE id = :id");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    qint32 I_end = end * 10000;

    query.bindValue(":data", ba);
    query.bindValue(":id", I_end);
    query.exec();
}


void DbHandler::appendImage(qreal start, qreal end, QImage image)
{
    QSqlQuery query(database);
    query.prepare("insert into BigImages (id, data) values (:id, :data)");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    qint32 I_end = end * 10000;

    query.bindValue(":id", I_end);
    query.bindValue(":data", ba);
    query.exec();
}

void DbHandler::insertImage(qint32 index, QImage image)
{
    QSqlQuery query(database);
    query.prepare("select * from BigImages where id >= :id order by id desc");

    qint32 I_index = (index+1) * 10000;

    query.bindValue(":id", I_index);
    query.exec();

    while (query.next())
    {
        QSqlQuery query1(database);
        query1.prepare("update BigImages set id = :newId, data = :data where id = :oldId");
        query1.bindValue(":newId", query.value("id").toInt() + 10000);
        query1.bindValue(":data", query.value("data"));
        query1.bindValue(":oldId", query.value("id"));
        query1.exec();
    }

    query.prepare("insert into BigImages (id, data) values (:id, :data)");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    query.bindValue(":id", I_index);
    query.bindValue(":data", ba);
    query.exec();
}


void DbHandler::updateImage(qreal start, qreal end, QImage image)
{
    QSqlQuery query(database);
    query.prepare("update BigImages set data = :data where id > :start and id <= :end");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    qint32 I_start = start * 10000;
    qint32 I_end = end * 10000;

    query.bindValue(":data", ba);
    query.bindValue(":start", I_start);
    query.bindValue(":end", I_end);
    query.exec();
}



void DbHandler::updateImage(qreal oldStart, qreal oldEnd, qreal newStart, qreal newEnd)
{
    QSqlQuery query(database);
    query.prepare("update BigImages set id = :newId where id > :oldStart and id <= :oldEnd");

    qint32 I_newEnd = newEnd * 10000;
    qint32 I_oldStart = oldStart * 10000;
    qint32 I_oldEnd = oldEnd * 10000;

    query.bindValue(":newId", I_newEnd);
    query.bindValue(":oldStart", I_oldStart);
    query.bindValue(":oldEnd", I_oldEnd);
    query.exec();
}



void DbHandler::updateImage(qreal oldStart, qreal oldEnd, qreal newStart, qreal newEnd, QImage image)
{
    QSqlQuery query(database);
    query.prepare("update BigImages set id = :newId, data = :newData where id > :oldStart and id <= :oldEnd");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");

    qint32 I_newEnd = newEnd * 10000;
    qint32 I_oldStart = oldStart * 10000;
    qint32 I_oldEnd = oldEnd * 10000;

    query.bindValue(":newId", I_newEnd);
    query.bindValue(":newData", ba);
    query.bindValue(":oldStart", I_oldStart);
    query.bindValue(":oldEnd", I_oldEnd);
    query.exec();
}



void DbHandler::deleteLastImage()
{
    QSqlQuery query(database);
    query.exec("select * from BigImages");
    if (query.last())
    {
        qint32 depth = query.value("id").toInt();
        query.prepare("delete from BigImages where id = :id");
        query.bindValue(":id", depth);
        query.exec();
    }
}


// 删除照片 同时修改之后的所有id
void DbHandler::deleteImage(qreal start, qreal end)
{
    QSqlQuery query(database);
    query.prepare("select * from BigImages where id > :start");
    query.bindValue(":start", start * 10000);
    query.exec();

    qint32 I_start = start * 10000;
    qint32 I_end = end * 10000;

    if (query.next())
    {
        QSqlQuery query1(database);
        query1.prepare("delete from BigImages where id > :start and id <= :end");
        query1.bindValue(":start", I_start);
        query1.bindValue(":end", I_end);
        query1.exec();


        while (query.next())
        {
            query1.prepare("update BigImages set id = :newId where id = :oldId");
            query1.bindValue(":oldId", query.value("id"));
            query1.bindValue(":newId", query.value("id").toInt()-10000);
            query1.exec();
        }
    }
}







void DbHandler::saveItem(QUuid uuid, qint32 index, qint32 depth, quint8 type, QString dataStr, QString remark)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO items (uuid, number, depth, type, data, remark) VALUES (:uuid, :number, :depth, :type, :data, :remark)");
    query.bindValue(":uuid", uuid.toString());
    query.bindValue(":number", index);
    query.bindValue(":depth", depth);
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


DbHandler::IndexData DbHandler::getIndexData(qint32 index)
{
    IndexData indexData;
    indexData.image = getBigImage(index);

    QSqlQuery query(database);
    query.prepare("SELECT * FROM items WHERE number = :number ORDER by depth ASC");
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





