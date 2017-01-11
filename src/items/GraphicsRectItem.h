#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include "GraphicsScene.h"
#include "GraphicsSettings.h"
#include "type.h"

class GraphicsRectItem : public QGraphicsRectItem
{
public:
    GraphicsRectItem(const QRectF &rect, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsRectItem();

    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);



    int type() const
    {
        return Rect;
    }

    QString getDataString();
    static GraphicsRectItem *loadFromString(QString data);

    void setFinished() {hasDrawed = true;}

    QString getRemark() {return remarkContent;}
    void setRemark(QString remark) {remarkContent = remark;}



protected:  
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;
    QPointF origPos;

    QString remarkContent;


};
