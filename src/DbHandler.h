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
#include <QBuffer>
#include <QUuid>

class DbHandler : public QObject
{
    Q_OBJECT

public:
    DbHandler(QObject *parent = Q_NULLPTR);
    ~DbHandler();

    // 工程属性
    typedef struct PrjInfo{
        bool isUp2Down;
        qreal startHeight;
        qreal endHeight;
        qreal diameter;
        QString projectName;
        QString projectTime;
        QString projectSite;
        QString orificeNumber;

        PrjInfo()
        {
            isUp2Down = false;
            startHeight = 0.0;
            endHeight = 0.0;
            diameter = 0;
        }

    }PrjInfo;


    typedef struct {
        qreal diameter;
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
    void setBigImage(qreal start, qreal end, QImage image);

    typedef struct {
        QUuid uuid;
        qint32 type;
        QString dataStr;
        QString remark;
    }ItemData;

    typedef struct {
        BigImage image;
        QVector<ItemData> itemDatas;
    }IndexData;


    void saveItem(QUuid uuid, quint16 index, quint8 type, QString dataStr, QString remark);
    IndexData getIndexData(quint16 index);

public slots:
    void setPrjInfo(DbHandler::PrjInfo prjInfo);

    void deleteItem(QUuid uuid);

    void updateItemremark(QUuid uuid, QString remark);

private:
    QSqlDatabase database;
    ErrorCode errorCode;

    QMap<QString, QString> projectInfoFields;

};

#endif // DBHANDLER_H
