#pragma once

#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QGraphicsSceneMouseEvent>

#include <QPainter>
#include <qmath.h>
#include "GraphicsLineItem.h"
#include "GraphicsScene.h"
#include "GraphicsSettings.h"

class GraphicsAngleItem : public QGraphicsPolygonItem
{
public:
    GraphicsAngleItem(const QPointF &point, QGraphicsItem * parent = Q_NULLPTR);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    QPainterPath shape() const;
    QRectF boundingRect() const;

protected:
    bool sceneEvent(QEvent *event);

private:
    QVector<QPointF> polygonPoints;
    bool secondPointFinished;
};

