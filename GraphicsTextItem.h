#pragma once

#include <QGraphicsTextItem>

class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
	GraphicsTextItem(const QPointF&, const QString& text = QString(), QGraphicsItem *parent = 0);
    ~GraphicsTextItem();
};
