#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QList>

#include "GraphicsTextItem.h"
#include "GraphicsLineItem.h"


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    enum Mode {MoveItem, InsertLine, InsertRuler, InsertRectangle, InsertAnyShape, InsertOccurance, InsertTextBox, InsertCross};

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
    QList<QGraphicsItem *> itemGroup;

    QGraphicsPixmapItem *pixmapItem;
    QGraphicsItem *item;
    QGraphicsItem *createNewItem(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void modeChanged(GraphicsScene::Mode curMode);

};
