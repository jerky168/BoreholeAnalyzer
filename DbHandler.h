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

    typedef enum {
        NoError,
        OpenFailed,
        NoProjectInfo,
        NoBigImages,
        NoImages
    }ErrorCode;

    typedef struct {
        double x, y;
        QString content;
    }TextItem;

    typedef struct {
        double x1, y1;
        double x2, y2;
    }WidthItem;

    typedef struct {
        double x1, y1;
        double x2, y2;
        double x3, y3;
    }AngleItem;


    bool openDatabase(QString filepath);
    void closeDatabase();
    bool isOpened();


    ErrorCode lastError() { return errorCode; }

    // get project infomation
    PrjInfo getPrjInfo();

    // get big images
    BigImage getBigImage(quint16 index);

    // get small images
    QVector<QPixmap> getSmallImage(quint32 start, quint32 end);

    // get all item
    QVector<TextItem> getTextItem();
    QVector<WidthItem> getWidthItem();
    QVector<AngleItem> getAngleItem();

    // save all item
    void saveTextItem(QVector<TextItem> items);
    void saveWidthItem(QVector<WidthItem> items);
    void saveWidthItem(QVector<AngleItem> items);

signals:

public slots:

private:
    QSqlDatabase database;
    ErrorCode errorCode;
};

#endif // DBHANDLER_H
