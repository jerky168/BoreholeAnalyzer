#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QUuid>


#include "DefectWidget.h"

#include "GraphicsSettings.h"
#include "GraphicsTextItem.h"
#include "GraphicsLineItem.h"
#include "GraphicsAngleItem.h"
#include "GraphicsRectItem.h"
#include "GraphicsAnyshape.h"
#include "GraphicsOccurance.h"


#define Border          150
#define Interval        30
#define Segment         30

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    enum Mode {MoveItem, InsertLine, InsertRuler, InsertShift, InsertRectangle, InsertAnyShape, InsertOccurance, InsertTextBox, InsertCross};

    void setCurMode(Mode mode);


    static Mode getCurMode() {return curMode;}
    static double getRatio() {return ratio;}

    void itemFinished(QString content);

    QImage getSceneImage();


    static QImage getImageFromData(QPixmap pixmap, qreal start, qreal end, QVector<DefectWidget::ItemData> items);


public slots:
    void clearScene();
    void updateIndexData(QPixmap pixmap, qreal start, qreal end, QVector<DefectWidget::ItemData> items);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void drawBackground(QPainter * painter, const QRectF & rect);

private:
    static Mode curMode;
    static qreal ratio;

    bool showInfo;
    qreal pixmap_start, pixmap_end;
    qreal pixmap_width, pixmap_height;

    QGraphicsItem *item;


signals:
    void modeChanged(GraphicsScene::Mode curMode); 
    void showStatus(QString message, int timeout);
    void showRealInfo(QString info);

    void itemInserted(QGraphicsItem *item, QUuid uuid);
};
