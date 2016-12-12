#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>
#include <QPixmap>
#include <QDateTime>
#include <QVector>

#include <QUuid>

#include "type.h"
#include "GraphicsAngleItem.h"
#include "GraphicsAnyshape.h"
#include "GraphicsLineItem.h"
#include "GraphicsRectItem.h"
#include "GraphicsTextItem.h"
#include "GraphicsOccurance.h"

#include "DefectWidget.h"


class DbHandler : public QObject
{
    Q_OBJECT

public:
    DbHandler(QObject *parent = Q_NULLPTR);
    ~DbHandler();

    typedef struct PrjInfo{
        bool isUp2Down;
        quint32 startHeight;
        quint32 endHeight;
        quint32 diameter;
        QString projectName;
        QDateTime projectTime;
        QString projectSite;

        PrjInfo()
        {
            isUp2Down = false;
            startHeight = 0;
            endHeight = 0;
            diameter = 0;
        }

    }PrjInfo;


    typedef struct {
        quint32 start;
        quint32 end;
        QPixmap pixmap;
    }BigImage;

    typedef struct {
        quint32 depth;
        QPixmap pixmap;
    }SmallImage;


    typedef struct {
        BigImage image;
        QVector<DefectWidget::ItemData> items;
    }IndexData;


    typedef enum {
        NoError,
        OpenFailed,
        NoProjectInfo,
        NoBigImages,
        NoImages
    }ErrorCode;


    bool openDatabase(QString filepath);
    void closeDatabase();
    bool isOpened();


    ErrorCode lastError() { return errorCode; }


    PrjInfo getPrjInfo();
    BigImage getBigImage(quint16 index);
    QVector<QPixmap> getSmallImage(quint32 start, quint32 end);


    void saveItem(quint16 index, QUuid uuid, QGraphicsItem *item);
    IndexData getIndexData(quint16 index);



private:
    QSqlDatabase database;
    ErrorCode errorCode;

};

#endif // DBHANDLER_H
