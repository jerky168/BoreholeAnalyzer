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


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(DbHandler *dbHandler, QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    enum Mode {MoveItem, InsertLine, InsertRuler, InsertShift, InsertRectangle, InsertAnyShape, InsertOccurance, InsertTextBox, InsertCross};

    void initItem();

    void setCurMode(Mode mode);
    Mode getCurMode() {return curMode;}

public slots:
    void updatePixmap(QPixmap pixmap);
    void clearScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    Mode curMode;

    DbHandler *handler;

    QVector<QGraphicsItem *> itemGroup;

    QGraphicsPixmapItem *pixmapItem;
    QGraphicsItem *item;
    QGraphicsItem *createNewItem(QGraphicsSceneMouseEvent *mouseEvent);


signals:
    void modeChanged(GraphicsScene::Mode curMode);

};
