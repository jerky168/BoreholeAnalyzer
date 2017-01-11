#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QtMath>

#include "GraphicsSettings.h"
#include "GraphicsScene.h"
#include "type.h"

class GraphicsAnyshape : public QGraphicsPolygonItem
{
public:
    GraphicsAnyshape(QPointF pos, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsAnyshape();

    int type() const
    {
        return AnyShape;
    }

    QString getDataString();
    static GraphicsAnyshape *loadFromString(QString data);


    void setFinished() {hasDrawed = true;}

    QString getRemark() {return remarkContent;}
    void setRemark(QString remark) {remarkContent = remark;}

protected:
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;

    QString remarkContent;

    qreal calcArea();
    void addPoint(QPointF pos);
    void updatePoint(QPointF pos);
    void clearPoints();

};

