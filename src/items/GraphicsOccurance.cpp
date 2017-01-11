#include "GraphicsOccurance.h"

GraphicsOccurance::GraphicsOccurance(const QLineF &line, QGraphicsItem *parent) :
    QGraphicsLineItem(line, parent),
    hasDrawed(false)
{
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
}

GraphicsOccurance::~GraphicsOccurance()
{

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



void GraphicsOccurance::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!hasDrawed)
    {
    	QLineF newLine(line().p1(), event->scenePos());
	    setLine(newLine);
	    return;
    }

	    
}

void GraphicsOccurance::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!hasDrawed)
    {
    	GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
	    scene->itemFinished(QString());
	    hasDrawed = true;
        return;
    }
}




QString GraphicsOccurance::getDataString()
{
    QString data;
    data.append(QString::number(line().x1() - Border, 'f', 2));
    data.append(",");
    data.append(QString::number(line().y1() - Border, 'f', 2));
    data.append(";");
    data.append(QString::number(line().x2() - Border, 'f', 2));
    data.append(",");
    data.append(QString::number(line().y2() - Border, 'f', 2));

    return data;
}

GraphicsOccurance * GraphicsOccurance::loadFromString(QString data)
{
    QString pos1_str = data.section(';', 0, 0);
    QString pos2_str = data.section(';', 1, 1);
    QPointF pos1, pos2;
    pos1.setX(pos1_str.section(',', 0, 0).toDouble() + Border);
    pos1.setY(pos1_str.section(',', 1, 1).toDouble() + Border);
    pos2.setX(pos2_str.section(',', 0, 0).toDouble() + Border);
    pos2.setY(pos2_str.section(',', 1, 1).toDouble() + Border);

    GraphicsOccurance *item = new GraphicsOccurance(QLineF(pos1, pos2));
    return item;
}


