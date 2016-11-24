#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>
#include <QPixmap>
#include <QDateTime>

class DbHandler : public QObject
{
    Q_OBJECT

public:
    DbHandler(QObject *parent = Q_NULLPTR);
    DbHandler(QString filename, QObject *parent = 0);
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

    typedef enum {
        NoError,
        OpenFailed,
        NoProjectInfo,
        NoBigImages,
        NoImages
    }ErrorCode;



    // open database
    void openDatabase(QString filepath);

    // return error code
    ErrorCode lastError() { return errorCode; }

    // get project infomation
    PrjInfo getPrjInfo();

    // get image
    BigImage getBigImage(quint16 index);


    QVector<QPixmap> getSmallImage(quint32 start, quint32 end);

signals:

public slots:

private:
    QSqlDatabase database;
    ErrorCode errorCode;
};

#endif // DBHANDLER_H
