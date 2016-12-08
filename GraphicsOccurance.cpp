#include "GraphicsOccurance.h"

GraphicsOccurance::GraphicsOccurance(const QLineF &line, QGraphicsItem *parent) :
    QGraphicsLineItem(line, parent)
{
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
}

GraphicsOccurance::~GraphicsOccurance()
{

}


QString GraphicsOccurance::getItemText()
{
    return QString("Occurance");
}


void GraphicsOccurance::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    widget = Q_NULLPTR;
    option = Q_NULLPTR;
    painter->setRenderHint(QPainter::Antialiasing);

    QPen thisPen;
    thisPen.setColor(Qt::yellow);
    thisPen.setWidth(pen().width());
    thisPen.setStyle(Qt::SolidLine);
    painter->setPen(thisPen);

    painter->drawLine(line().p1(), line().p2());
}


void GraphicsOccurance::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void GraphicsOccurance::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QLineF newLine(line().p1(), event->scenePos());
    setLine(newLine);
}

void GraphicsOccurance::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
    scene->itemFinished(QString());
}



GraphicsOccurance::Data GraphicsOccurance::getData()
{
    Data data;
    data.points[0] = this->line().p1();
    data.points[1] = this->line().p2();
    return data;
}

