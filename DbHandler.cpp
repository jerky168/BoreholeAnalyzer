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





QVector<DbHandler::TextItem> DbHandler::getTextItem()
{
    QVector<TextItem> items;

    QSqlQuery query(database);
    query.exec("select * from TextItem");

    TextItem item;
    while (!query.next())
    {
        item.x = query.value(0).toDouble();
        item.y = query.value(1).toDouble();
        item.content = query.value(2).toString();
        items.append(item);
    }
    return items;
}



QVector<DbHandler::WidthItem> DbHandler::getWidthItem()
{
    QVector<WidthItem> items;

    QSqlQuery query(database);
    query.exec("select * from WidthItem");

    WidthItem item;
    while(!query.next())
    {
        item.x1 = query.value(0).toDouble();
        item.y1 = query.value(1).toDouble();
        item.x2 = query.value(2).toDouble();
        item.y2 = query.value(3).toDouble();
        items.append(item);
    }
    return items;
}

QVector<DbHandler::AngleItem> DbHandler::getAngleItem()
{
    QVector<AngleItem> items;

    QSqlQuery query(database);
    query.exec("select * from AngleItem");

    AngleItem item;
    while(!query.next())
    {
        item.x1 = query.value(0).toDouble();
        item.y1 = query.value(1).toDouble();
        item.x2 = query.value(2).toDouble();
        item.y2 = query.value(3).toDouble();
        item.x3 = query.value(4).toDouble();
        item.y3 = query.value(5).toDouble();
        items.append(item);
    }
    return items;
}


void DbHandler::saveTextItem(QVector<TextItem> items)
{
    QSqlQuery query(database);

    query.prepare("insert into TextItem values (?, ?, ?)");
    for (int i = 0; i < items.count(); i++)
    {
        query.bindValue(0, items[i].x);
        query.bindValue(1, items[i].y);
        query.bindValue(2, items[i].content);
        query.exec();
    }
}

void DbHandler::saveWidthItem(QVector<WidthItem> items)
{
    QSqlQuery query(database);

    query.prepare("insert into TextItem values (?, ?, ?, ?, ?)");
    for (int i = 0; i < items.count(); i++)
    {
        query.bindValue(0, items[i].x1);
        query.bindValue(1, items[i].y1);
        query.bindValue(2, items[i].x2);
        query.bindValue(3, items[i].y2);
        query.exec();
    }
}

