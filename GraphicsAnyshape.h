#pragma once

#include <QGraphicsPolygonItem>

#include "GraphicsSettings.h"
#include "GraphicsScene.h"

#include "type.h"

class GraphicsAnyshape : public QGraphicsPolygonItem
{
public:
    GraphicsAnyshape(QPointF pos, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsAnyshape();

    typedef struct
    {
        QPolygonF polygon;
    }Data;

    int type() const
    {
        return AnyShape;
    }
    Data getData();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;

    void addPoint(QPointF pos);
    void updatePoint(QPointF pos);
    void clearPoints();

};

