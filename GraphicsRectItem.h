#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtMath>

#include "GraphicsScene.h"
#include "GraphicsSettings.h"

class GraphicsRectItem : public QGraphicsRectItem
{
public:
    GraphicsRectItem(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsRectItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

protected:  
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;
    QPointF origPos;

};
