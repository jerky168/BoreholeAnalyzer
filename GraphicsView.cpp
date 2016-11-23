#include "GraphicsView.h"

GraphicsView::GraphicsView( QWidget * parent ) :
    QGraphicsView(parent),
    scene(new GraphicsScene())
{
    setDragMode(NoDrag);
	setMouseTracking(true);
	totalFactor = 1;
	scale(0.23, 0.23);

    setScene(scene);
}

GraphicsView::~GraphicsView()
{

}


void GraphicsView::wheelEvent(QWheelEvent *event)
{
	double numDegress = event->delta() / 8.0;
	double numSteps = numDegress / 15.0;
    factor = qPow(1.125, numSteps);

	totalFactor *= factor;	
	scale(factor, factor);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mouseMoveEvent(mouseEvent);
}
