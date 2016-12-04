#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QVector>

#include "DbHandler.h"
#include "GraphicsTextItem.h"
#include "GraphicsLineItem.h"
#include "GraphicsAngleItem.h"
#include "GraphicsRectItem.h"
#include "GraphicsAnyshape.h"


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(DbHandler *dbHandler, QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    enum Mode {MoveItem, InsertLine, InsertRuler, InsertShift, InsertRectangle, InsertAnyShape, InsertOccurance, InsertTextBox, InsertCross};

    void initItem();

    void setCurMode(Mode mode);

    static Mode getCurMode() {return curMode;}
    static double getRatio() {return ratio;}

public slots:
    void updatePixmap(QPixmap pixmap);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    // draw background
    void drawBackground(QPainter * painter, const QRectF & rect);

private:
    DbHandler *handler;

    // mode variable
    static Mode curMode;
    // most important variable
    static double ratio;



    QGraphicsItem *item;
    QGraphicsItem *createNewItem(QGraphicsSceneMouseEvent *mouseEvent);


signals:
    void modeChanged(GraphicsScene::Mode curMode);

};
