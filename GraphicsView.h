#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QPinchGesture>
#include <QtMath>
#include <QPainter>

#include <QGraphicsSceneMouseEvent>
#include "GraphicsScene.h"


class GraphicsView : public QGraphicsView
{
	Q_OBJECT
		
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
    ~GraphicsView();

    GraphicsScene *scene;

protected:
	void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
	double factor;
	double totalFactor;


};
