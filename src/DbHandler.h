#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QDateTime>
#include <QVector>

#include <QUuid>




class DbHandler : public QObject
{
    Q_OBJECT

public:
    DbHandler(QObject *parent = Q_NULLPTR);
    ~DbHandler();

    typedef struct PrjInfo{
        bool isUp2Down;
        qreal startHeight;
        qreal endHeight;
        quint32 diameter;
        QString projectName;
        QDateTime projectTime;
        QString projectSite;

        PrjInfo()
        {
            isUp2Down = false;
            startHeight = 0.0;
            endHeight = 0.0;
            diameter = 0;
        }

    }PrjInfo;


    typedef struct {
        qreal start;
        qreal end;
        QPixmap pixmap;
    }BigImage;





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


    typedef struct {
        QUuid uuid;
        qint32 type;
        QString dataStr;
    }ItemData;

    typedef struct {
        BigImage image;
        QVector<ItemData> itemDatas;
    }IndexData;


    void saveItem(QUuid uuid, quint16 index, quint8 type, QString dataStr);
    IndexData getIndexData(quint16 index);



private:
    QSqlDatabase database;
    ErrorCode errorCode;

};

#endif // DBHANDLER_H
