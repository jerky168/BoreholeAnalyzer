#include "GraphicsView.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QPinchGesture>
#include <qmath.h>
#include <QPainter>

GraphicsView::GraphicsView( QWidget * parent  )
	:QGraphicsView(parent)
{
	setDragMode(ScrollHandDrag);
	setMouseTracking(true);
	totalFactor = 1;
	scale(0.23, 0.23);
}

void GraphicsView::wheelEvent( QWheelEvent *event )
{
	double numDegress = event->delta() / 8.0;
	double numSteps = numDegress / 15.0;
    factor = qPow(1.125, numSteps);

	totalFactor *= factor;	
	scale(factor, factor);
}

