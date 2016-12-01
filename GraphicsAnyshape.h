#pragma once

#include <QGraphicsPolygonItem>


#include "GraphicsScene.h"

class GraphicsAnyshape : public QGraphicsPolygonItem
{
public:
    GraphicsAnyshape();

protected:
    bool sceneEvent(QEvent *event);

private:
    bool isCurrentMode();
};

