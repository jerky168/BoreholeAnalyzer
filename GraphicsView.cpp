#include "GraphicsView.h"

GraphicsView::GraphicsView( QWidget * parent ) :
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
	setMouseTracking(true);
	totalFactor = 1;
	scale(0.23, 0.23);
}

GraphicsView::~GraphicsView()
{

}

// 当scene编辑模式发生改变时 鼠标样式放生改变
void GraphicsView::handleModeChanged(GraphicsScene::Mode lastMode, GraphicsScene::Mode curMode)
{
    if (curMode != GraphicsScene::Normal)
    {
        setDragMode(QGraphicsView::NoDrag);
    }
    else
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


// 当插入item后
void GraphicsView::handleItemInserted(QGraphicsItem * const &insertedItem)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}


// 当删除item后
void GraphicsView::handleItemDeleted(QGraphicsItem* const &deletedItem)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
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
    if (((GraphicsScene *)scene())->getCurMode() == GraphicsScene::MoveItem)
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
