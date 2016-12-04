#include "GraphicsAnyshape.h"

GraphicsAnyshape::GraphicsAnyshape(QPointF pos, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent),
    hasDrawed(false),
    drawLine(true),
    hasEnded(false)
{
    qDebug() << polygon.count();
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

    painter->drawPolygon(polygon());

    if (hasEnded)
    {
        return;
    }


}



void GraphicsAnyshape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 如果已经画完 则不对鼠标事件进行反应
    if (hasDrawed)
        return;

    // 如果按下的是右键 结束绘画
    if (event->buttons() & Qt::RightButton)
    {
        hasDrawed = true;
        return;
    }

    // 如果按下的是左键 添加新的点
    addPoint(event->pos());
}


void GraphicsAnyshape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (hasDrawed)
        return;

    updatePoint(event->pos());
}

void GraphicsAnyshape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

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

