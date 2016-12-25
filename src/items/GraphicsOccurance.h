#pragma once

#include <QGraphicsLineItem>

#include "GraphicsSettings.h"
#include "GraphicsScene.h"

class GraphicsOccurance : public QGraphicsLineItem
{
public:
    GraphicsOccurance(const QLineF &line, QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsOccurance();

    typedef struct
    {
        QPointF points[2];
    }Data;

    int type() const
    {
        return Occurance;
    }
    Data getData();
    QString getContent() {return content;}

    QString getDataString();
    static GraphicsOccurance *loadFromString(QString data);

    void setFinished() {hasDrawed = true;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;
    QString content;
};
