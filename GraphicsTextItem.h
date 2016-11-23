#pragma once

#include "GraphicsSettings.h"
#include <QGraphicsSimpleTextItem>


class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
	GraphicsTextItem(const QPointF&, const QString& text = QString(), QGraphicsItem *parent = 0);
    ~GraphicsTextItem();
};
