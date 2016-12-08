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

//    // 查询是否有文本item
//    if (!query.exec("select * from TextItem"))
//    {
//        query.exec("create table TextItem(x real, y real, content text)");
//    }

//    // 查询是否有缝宽item
//    if (!query.exec("select * from WidthItem"))
//    {
//        query.exec("create table WidthItem(x1 real, y1 real, x2 real, y2 real)");
//    }

//    // 查询是否有角度item
//    if (!query.exec("select * from AngleItem"))
//    {
//        query.exec("create table AngleItem(x1 real, y1 real, x2 real, y2 real, x3 real, y3 real)");
//    }
    if (!query.exec("select * from Items"))
    {
        query.exec("creat table ItemsData(id INTEGER KEY PRIMARY KEY autoincrement, type VARCHAR,data VARCHAR)");
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



DbHandler::DisplayData DbHandler::getDisplayData(quint16 index)
{
    QSqlQuery query(database);
    query.prepare("select * from bigImages where id > ? and id <= ?");
    query.bindValue(0, index * 10000);
    query.bindValue(1, (index + 1) * 10000);
    query.exec();
    query.first();

    DisplayData displayData;
    displayData.bigImage.start = index*10000;
    displayData.bigImage.end = query.value(0).toInt();
    QByteArray imgData = query.value(1).toByteArray();
    displayData.bigImage.pixmap.loadFromData(imgData);

    query.exec("select * from items");
    while(!query.next())
    {
//        QUuid uuid(query.value(0).toString());
//        ItemType type = query.value(1).toInt();
//        QString data = query.value(2).toString();
//        switch(type)
//        {
//            default:
//                break;
//        }
    }
    return displayData;
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


QVector<QString> DbHandler::getItem(QGraphicsItem *item)
{
    QSqlQuery query(database);

    QJsonObject json;
    QJsonDocument document, parse_document;
    QJsonParseError json_error;

    GraphicsAngleItem *angleitem;
    GraphicsAngleItem::Data angleData;
    QString angleJson;


    GraphicsAnyshape *anyShapeItem;
    GraphicsAnyshape::Data anyShapeData;
    QString anyShapeJson;


    GraphicsLineItem *ruleritem;
    GraphicsLineItem::Data rulerData;
    QString rulerJson;

    GraphicsTextItem *textItem;
    GraphicsTextItem::Data textData;


    GraphicsRectItem *rectItem;
    GraphicsRectItem::Data rectData;
    QString rectJson;

    GraphicsOccurance *occuranceItem;
    GraphicsOccurance::Data occuranceData;
    QString occuranceJson;

    QByteArray byte_array;

    QVector<QString> stringVector;

    switch(item->type())
    {
    case Angle:
        angleitem  = dynamic_cast<GraphicsAngleItem *>(item);
        angleData = angleitem->getData();


        json.insert("point0.x", angleData.points[0].x());
        json.insert("point0.y", angleData.points[0].y());

        json.insert("point1.x", angleData.points[1].x());
        json.insert("point1.y", angleData.points[1].y());

        json.insert("point2.x", angleData.points[2].x());
        json.insert("point2.y", angleData.points[2].y());

        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        angleJson = QString(byte_array);


        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "Angle");
        query.bindValue(":data", angleJson);
        query.exec();

        break;


    case AnyShape:
        anyShapeItem = dynamic_cast<GraphicsAnyshape *>(item);
        anyShapeData = anyShapeItem->getData();

        json.insert("polygon.x", anyShapeData.polygon.constData()->x());
        json.insert("polygon.y", anyShapeData.polygon.constData()->y());


        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        anyShapeJson = QString(byte_array);


        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "AnyShape");
        query.bindValue(":data", anyShapeJson);
        query.exec();


        break;

    case Ruler:
        ruleritem = dynamic_cast<GraphicsLineItem *>(item);
        rulerData = ruleritem->getData();

        json.insert("point0.x", rulerData.points[0].x());
        json.insert("point0.y", rulerData.points[0].y());
        json.insert("point1.x", rulerData.points[1].x());
        json.insert("point1.y", rulerData.points[1].y());

        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        rulerJson = QString(byte_array);

        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "Ruler");
        query.bindValue(":data", rulerJson);
        query.exec();

        break;

    case Occurance:
        occuranceItem = dynamic_cast<GraphicsOccurance *>(item);
        occuranceData = occuranceItem->getData();

        json.insert("point0.x", occuranceData.points[0].x());
        json.insert("point0.y", occuranceData.points[0].y());

        json.insert("point1.x", occuranceData.points[1].x());
        json.insert("point1.y", occuranceData.points[1].y());

        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        occuranceJson = QString(byte_array);


        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "Occurance");
        query.bindValue(":data", occuranceJson);
        query.exec();

        break;

    case Rect:
        rectItem = dynamic_cast<GraphicsRectItem *>(item);
        rectData = rectItem->getData();

        json.insert("point0.x", rectData.points[0].x());
        json.insert("point0.y", rectData.points[0].y());

        json.insert("point1.x", rectData.points[1].x());
        json.insert("point1.y", rectData.points[1].y());


        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        rectJson = QString(byte_array);

        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "Rect");
        query.bindValue(":data", rectJson);
        query.exec();


        break;
    case Text:
        textItem = dynamic_cast<GraphicsTextItem *>(item);
        textData = textItem->getData();

        json.insert("point.x", textData.point.x());
        json.insert("point.y", textData.point.x());
        json.insert("content", textData.content);

        document.setObject(json);
        byte_array = document.toJson(QJsonDocument::Compact);
        QString textJson(byte_array);

        query.prepare("INSERT INTO ItemsData (type, data) VALUES (:type, :data)");
        query.bindValue(":type", "Text");
        query.bindValue(":data", textJson);
        query.exec();
        break;
    }

    //get data from DB
    query.exec("SELECT type, data FROM ItemsData");
    while (query.next())
    {
        QString type = query.value(0).toString();
        QString data = query.value(1).toString();
        parse_document = QJsonDocument::fromJson(data.toUtf8(), &json_error);

        if(json_error.error == QJsonParseError::NoError)
        {
            if(parse_document.isObject())
            {
                QVariantMap result = parse_document.toVariant().toMap();

            }
        }
        else
        {

        }
        qDebug() << type << ":" << data;

    }
    query.finish();
    return stringVector;
}






