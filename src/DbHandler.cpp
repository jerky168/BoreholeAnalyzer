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

    query.exec("select * from ProjectInfo");
    query.first();
    prjInfo.startHeight = (qreal)query.value(1).toInt() / 10000;


    query.exec("select * from bigImages");
    query.last();
    prjInfo.endHeight = (qreal)query.value(0).toInt() / 10000;

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
    bigImage.start = index;
    bigImage.end = (qreal)query.value(0).toInt() / 10000.0;
    QByteArray imgData = query.value(1).toByteArray();
    bigImage.pixmap.loadFromData(imgData);

    return bigImage;
}



void DbHandler::saveItem(quint16 index, QUuid uuid, QGraphicsItem *item)
{
    QString dataStr;
    switch (item->type())
    {
        case Angle:
        {
            GraphicsAngleItem *i = dynamic_cast<GraphicsAngleItem *>(item);
            dataStr = i->getDataString();
            break;
        }

        case AnyShape:
        {
            GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
            dataStr = i->getDataString();
            break;
        }
        case Ruler:
        {
            GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
            dataStr = i->getDataString();
            break;
        }
        case Occurance:
        {
            GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
            dataStr = i->getDataString();
            break;
        }
        case Rect:
        {
            GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
            dataStr = i->getDataString();
            break;
        }

        case Text:
        {
            GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
            dataStr = i->getDataString();
            break;
        }

        default:
        {
            break;
        }
    }

    QSqlQuery query(database);
    query.prepare("INSERT INTO items (uuid, number, type, data) VALUES (:uuid, :number, :type, :data)");
    query.bindValue(":uuid", uuid.toString());
    query.bindValue(":number", index);
    query.bindValue(":type", item->type());
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

    DefectWidget::ItemData itemData;
    while(query.next())
    {
        itemData.uuid = QUuid(query.value(0).toString());
        int type = query.value(1).toInt();
        switch (type)
        {
            case Angle:
            {
                GraphicsAngleItem *i = GraphicsAngleItem::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            case AnyShape:
            {
                GraphicsAnyshape *i = GraphicsAnyshape::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            case Ruler:
            {
                GraphicsLineItem *i = GraphicsLineItem::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            case Occurance:
            {
                GraphicsOccurance *i = GraphicsOccurance::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            case Rect:
            {
                GraphicsRectItem *i = GraphicsRectItem::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            case Text:
            {
                GraphicsTextItem *i = GraphicsTextItem::loadFromString(query.value(2).toString());
                itemData.item = i;
                i->setFinished();
                break;
            }
            default:
                break;
        }
        indexData.items.append(itemData);
    }

    return indexData;
}


QImage DbHandler::getSceneImage(quint16 index)
{
    IndexData data = getIndexData(index);
    return GraphicsScene::getImageFromData(data.image.pixmap, data.image.start, data.image.end, data.items);
}




