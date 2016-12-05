#pragma once

#include <QGraphicsPolygonItem>

#include "GraphicsSettings.h"
#include "GraphicsScene.h"

class GraphicsAnyshape : public QGraphicsPolygonItem
{
public:
    GraphicsAnyshape(QPointF pos, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsAnyshape();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


private:
    bool hasDrawed, hasEnded;

    void clearPoints();
    void addPoint(QPointF pos);
    void updatePoint(QPointF pos);

};

