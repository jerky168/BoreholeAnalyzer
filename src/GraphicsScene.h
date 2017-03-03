#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QUuid>
#include <QtMath>
#include <QMap>

#include "type.h"
#include "GraphicsSettings.h"
#include "GraphicsTextItem.h"
#include "GraphicsLineItem.h"
#include "GraphicsRectItem.h"
#include "GraphicsAnyshape.h"
#include "GraphicsOccurance.h"




#define Border          200
#define Interval        30
#define Segment         30

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    typedef struct
    {
        QUuid uuid;
        QGraphicsItem *item;
    }ItemData;

    typedef struct
    {
        qreal depth;
        QString type;
        QString isSaved;
        QString data;
        QString remark;
        QUuid uuid;
    }TableData;


    enum Mode {MoveItem, InsertLine, InsertRuler, InsertShift, InsertRectangle, InsertAnyShape, InsertOccurance, InsertTextBox, InsertCross};

    void setCurMode(Mode mode);
    static Mode getCurMode();


    void itemFinished(QString content);
    void itemAborted();

    QImage getPixmapImage();
    QImage getSceneImage();
    QImage getSceneImageFor3D();
    QVector<TableData> getSavedTableData();

    static QImage getImageFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items);
    static QImage getPixmapImageFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items);
    static QVector<TableData> getTableDataFromData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items);

    QPointF scene2Real(QPointF scenePos);
    QPointF real2Scene(QPointF realPos);


    bool hasNewItem();
    QMap<QString, QGraphicsItem *> getNewItems();
    void saveNewItems();


    QStringList getAllItemString();

    void updateTable();

    qreal getItemDepth(QGraphicsItem *item);

public slots:
    void clearScene();
    void updateIndexData(QPixmap pixmap, qreal start, qreal end, qreal diameter, QMap<QString, QGraphicsItem *> items);
    void deleteItemData(QUuid uuid);

    void updateItemRemark(QUuid uuid, QString remark);



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    bool showInfo;  // 实时信息栏是否显示鼠标当前位置
    qreal pixmap_diameter;  // 直径
    qreal pixmap_start, pixmap_end; // 照片的起始深度和终止神父
    qreal pixmap_width, pixmap_height;  // 照片的宽度和高度
    QGraphicsItem *item;    // 当前正在绘制的item
    static Mode curMode;    // 当前的状态


    QString getShowString(QGraphicsItem *item);
    QString getAngleString(qreal angle);


    QMap<QString, QGraphicsItem *> newItems, savedItems;    // 用于保存新添加的items和已经保存的items
    void addItemData(QUuid uuid, QGraphicsItem *item, bool saved = false);

    void clearItemData();


signals:
    void modeChanged(GraphicsScene::Mode curMode);
    void showStatus(QString message);
    void showRealInfo(QString info);

    void emitTableData(QVector<GraphicsScene::TableData> tableData);
    void update3DImage(QImage image, qreal start, qreal end);

    void deleteSavedItem(QUuid uuid);
    void updateSavedItemRemark(QUuid uuid, QString remark);
};
