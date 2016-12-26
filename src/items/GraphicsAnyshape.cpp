#include "GraphicsAnyshape.h"

GraphicsAnyshape::GraphicsAnyshape(QPointF pos, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent),
    hasDrawed(false),
    content(QString())
{
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

}



void GraphicsAnyshape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (hasDrawed)
        return;

    if (event->buttons() & Qt::RightButton)
    {
        if (polygon().count() <= 2)
            return;

        hasDrawed = true;
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        content = QString::number(calcArea(), 'f', 2).append("cm2");
        scene->itemFinished(content);

        return;
    }

    addPoint(event->scenePos());
}


void GraphicsAnyshape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (hasDrawed)
        return;

    updatePoint(event->scenePos());
}


void GraphicsAnyshape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (hasDrawed)
        return;
}



qreal GraphicsAnyshape::calcArea()
{
    qreal area = 0.0;
    qreal x1, y1, x2, y2;

    QPolygonF polygon = this->polygon();
    polygon.append(polygon.first());
    int count = polygon.count();


    for (int i = 0; i < count - 1; i++)
    {
        x1 = polygon.at(i).x();
        y1 = polygon.at(i).y();
        x2 = polygon.at(i+1).x();
        y2 = polygon.at(i+1).y();
        area += x1 * y2 - x2 * y1;
    }

    area = qFabs(area);
    area /= 2 * qPow(GraphicsSettings::instance()->getRatio(), 2);
    area *= 10000;

    return area;
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

void GraphicsAnyshape::clearPoints()
{
    QPolygonF polygon;
    polygon.clear();
    setPolygon(polygon);
}


GraphicsAnyshape::Data GraphicsAnyshape::getData()
{
    Data data = {this->polygon()};
    return data;
}


QString GraphicsAnyshape::getContent()
{
    return content;
}

QString GraphicsAnyshape::getDataString()
{
    QString data = QString::number(this->polygon().count());
    for (int i = 0; i < polygon().count(); i++)
    {
        data.append(";");
        QPointF point = polygon().at(i);
        data.append(QString::number(point.x() - Border, 'f', 2));
        data.append(",");
        data.append(QString::number(point.y() - Border, 'f', 2));
    }

    qDebug() << polygon();
    return data;
}

GraphicsAnyshape * GraphicsAnyshape::loadFromString(QString data)
{
    int count = data.section(';', 0, 0).toInt();
    QPolygonF polygon;

    for (int i = 1; i <= count; i++)
    {
        QString section = data.section(';', i, i);
        qreal x = section.section(',', 0, 0).toDouble() + Border;
        qreal y = section.section(',', 1, 1).toDouble() + Border;
        QPointF point(x, y);
        polygon << point;
    }

    GraphicsAnyshape *item = new GraphicsAnyshape(QPoint());
    item->setPolygon(polygon);

    qDebug() << item->polygon();
    return item;
}
