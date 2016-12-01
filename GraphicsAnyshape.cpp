#include "GraphicsAnyshape.h"

GraphicsAnyshape::GraphicsAnyshape()
{

}



bool GraphicsAnyshape::sceneEvent(QEvent *event)
{
    QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent *>(event);

    if (isCurrentMode())
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



bool GraphicsAnyshape::isCurrentMode()
{
    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
    return (scene->getCurMode() == GraphicsScene::InsertRectangle);
}
