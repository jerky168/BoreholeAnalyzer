#pragma once

<<<<<<< 756f4e16bab91d7d8128f07f75bc6438a70bf51a
#include <QGraphicsTextItem>
=======
#include "GraphicsSettings.h"
#include <QGraphicsSimpleTextItem>
>>>>>>> 参考老版本的架构，继承Qt提供的基类，编写了不同功能的类，重写了view和scene的鼠标和滚轮事件。

class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
	GraphicsTextItem(const QPointF&, const QString& text = QString(), QGraphicsItem *parent = 0);
    ~GraphicsTextItem();
};
