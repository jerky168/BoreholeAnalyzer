#include "GraphicsView.h"

GraphicsView::GraphicsView( QWidget * parent ) :
    QGraphicsView(parent),
    isCross(false)
{
    setDragMode(ScrollHandDrag);
	setMouseTracking(true);
    //totalFactor = 1;
	scale(0.23, 0.23);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::handleModeChanged(GraphicsScene::Mode curMode)
{
    if (curMode != GraphicsScene::MoveItem)
    {
        if (isCross)
            setCursor(QCursor(Qt::CrossCursor));
        else
            setDragMode(QGraphicsView::NoDrag);
    }
    else
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }

}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
	double numDegress = event->delta() / 8.0;
	double numSteps = numDegress / 15.0;
    factor = qPow(1.125, numSteps);

    //totalFactor *= factor;
	scale(factor, factor);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());

    if (scene->getCurMode() == GraphicsScene::MoveItem)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        setDragMode(QGraphicsView::NoDrag);
    }

    emit mouseMoved(mouseEvent);

    QGraphicsView::mouseMoveEvent(mouseEvent);
}

void GraphicsView::handleZoomIn()
{
    factor = qPow(1.125, 1);
    scale(factor, factor);
}

void GraphicsView::handleZoomOut()
{
    factor = qPow(1.125, -1);
    scale(factor, factor);
}

void GraphicsView::handleCrossMouse(bool checked)
{
    if (checked)
    {
        setCursor(QCursor(Qt::CrossCursor));
        isCross = true;
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
        isCross = false;
    }
}
