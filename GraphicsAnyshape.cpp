#include "GraphicsAnyshape.h"

GraphicsAnyshape::GraphicsAnyshape()
{

}


void GraphicsAnyshape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void GraphicsAnyshape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void GraphicsAnyshape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}


bool GraphicsAnyshape::sceneEvent(QEvent *event)
{
    QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent *>(event);

    if (GraphicsScene::getCurMode() == GraphicsScene::InsertAnyShape)
    {
        switch (event->type())
        {
            case QEvent::GraphicsSceneMousePress :
            {
                break;
            }

            case QEvent::GraphicsSceneMouseMove :
            {

            }

            case QEvent::GraphicsSceneMouseRelease :
            {

            }

            default:
            {
                return false;
            }

        }
    }
    return true;
}



