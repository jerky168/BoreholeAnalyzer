#pragma once

#include <QGraphicsLineItem>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <qmath.h>

#include <QGraphicsSimpleTextItem>

#include "GraphicsScene.h"
#include "GraphicsSettings.h"
#include "stable.h"

#include "type.h"

class GraphicsLineItem : public QGraphicsLineItem
{
public:
    GraphicsLineItem(const QLineF & line, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsLineItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    QPainterPath shape() const;
    static QPainterPath shapeFromPath(const QPainterPath&,const QPen&);


    typedef struct
    {
        QPointF points[2];
    }Data;

    int type() const
    {
        return Ruler;
    }
    Data getData();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
    bool sceneEvent(QEvent *event);

private:


};
