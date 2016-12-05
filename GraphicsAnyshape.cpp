#include "GraphicsAnyshape.h"

GraphicsAnyshape::GraphicsAnyshape(QPointF pos, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent),
    hasDrawed(false),
    hasEnded(false)
{
    addPoint(pos);
    addPoint(pos);

    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
}

GraphicsAnyshape::~GraphicsAnyshape()
{

}


void GraphicsAnyshape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option = Q_NULLPTR;
    widget = Q_NULLPTR;
    painter->setRenderHint(QPainter::Antialiasing);

    QPen thisPen;
    thisPen.setColor(Qt::yellow);
    thisPen.setWidth(pen().width());
    thisPen.setStyle(Qt::SolidLine);
    painter->setPen(thisPen);

    painter->drawPolygon(this->polygon());

    if (hasEnded)
    {
        return;
    }
}



void GraphicsAnyshape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "item press event";
    if (hasDrawed)
        return;

    if (event->button() == Qt::RightButton)
    {
        hasDrawed = true;
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        scene->itemInserted();

        return;
    }

    addPoint(event->pos());
}


void GraphicsAnyshape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "item move event";
    if (hasDrawed)
        return;

    updatePoint(event->pos());
}


void GraphicsAnyshape::clearPoints()
{
    QPolygonF polygon;
    polygon.clear();
    setPolygon(polygon);
}

void GraphicsAnyshape::addPoint(QPointF pos)
{
    QPolygonF polygon = this->polygon();
    polygon.append(pos);
    setPolygon(polygon);
}

void GraphicsAnyshape::updatePoint(QPointF pos)
{
    QPolygonF polygon = this->polygon();
    polygon.removeLast();
    polygon.append(pos);
    setPolygon(polygon);
}

