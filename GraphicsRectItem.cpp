#include "GraphicsRectItem.h"

GraphicsRectItem::GraphicsRectItem(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent)
{
    setRect(rect);
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

GraphicsRectItem::~GraphicsRectItem()
{

}


void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    widget = Q_NULLPTR;
    option = Q_NULLPTR;
    painter->setRenderHint(QPainter::Antialiasing);

    QPen thisPen;
    thisPen.setColor(pen().color());
    thisPen.setWidth(pen().width());
    thisPen.setStyle(Qt::SolidLine);
    painter->setPen(thisPen);

    painter->drawRect(rect());
}


void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event = Q_NULLPTR;
    if (isSelected())
    {
        setFlag(QGraphicsItem::ItemIsMovable);
    }
}

void GraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    event = Q_NULLPTR;
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

bool GraphicsRectItem::sceneEvent(QEvent *event)
{
    QGraphicsSceneMouseEvent *e = dynamic_cast<QGraphicsSceneMouseEvent *>(event);

    if (isCurrentMode())
    {
        switch(event->type())
        {
            case QEvent::GraphicsSceneMousePress :
            {
                GraphicsSettings::instance()->setIsDrawing(true);
                break;
            }

            case QEvent::GraphicsSceneMouseMove :
            {
                QRectF newRect(rect().topLeft(), e->scenePos());
                setRect(newRect);
                break;
            }

            case QEvent::GraphicsSceneMouseRelease :
            {
                GraphicsSettings::instance()->setIsDrawing(false);
                break;
            }

            default:
            {
                return false;
            }
        }
    }
    else
    {
        QGraphicsItem::sceneEvent(event);
    }
    return true;
}


bool GraphicsRectItem::isCurrentMode()
{
    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
    return (scene->getCurMode() == GraphicsScene::InsertRectangle);
}

