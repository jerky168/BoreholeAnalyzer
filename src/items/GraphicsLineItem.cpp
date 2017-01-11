#include "GraphicsLineItem.h"

GraphicsLineItem::GraphicsLineItem(const QLineF &line, QGraphicsItem *parent) :
    QGraphicsLineItem(line, parent),
    hasDrawed(false)
{
    setLine(line);
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), GraphicsSettings::instance()->getPenWidth()));
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

GraphicsLineItem::~GraphicsLineItem()
{

}


void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    widget = Q_NULLPTR;
    option = Q_NULLPTR;
    painter->setRenderHint(QPainter::Antialiasing);

    QPen thisPen;
    thisPen.setColor(Qt::yellow);
    thisPen.setWidth(pen().width());
    thisPen.setStyle(Qt::SolidLine);
    painter->setPen(thisPen);

    if (this->isSelected())
    {
        thisPen.setStyle(Qt::DashLine);
        painter->setPen(thisPen);
    }
    painter->drawLine(line().p1(), line().p2());

}


QPainterPath GraphicsLineItem::shape() const
{
    QPainterPath path;
    if (line() == QLineF())
        return path;
    if (line().p1() == line().p2())
    {
        path.addEllipse(line().p1(), 2, 2);
        return path;
    }
    QLineF tempLine(line().p1(), line().p2());
    path.moveTo(line().p1());
    path.lineTo(line().p2());

    return shapeFromPath(path, pen());
}


void GraphicsLineItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
    event = Q_NULLPTR;
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void GraphicsLineItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    event = Q_NULLPTR;
    if (isSelected())
    {
        setFlag(QGraphicsItem::ItemIsMovable);
    }
}


QPainterPath GraphicsLineItem::shapeFromPath(const  QPainterPath& p,const QPen& pen)
{
    const qreal penWidthZero = qreal(0.00000001);
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(20);
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath path = ps.createStroke(p);
    path.addPath(p);
    return path;
}


bool GraphicsLineItem::sceneEvent(QEvent *event)
{
    QGraphicsSceneMouseEvent* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);

    if (GraphicsScene::getCurMode() == GraphicsScene::InsertLine)
    {
        switch (event->type())
        {
            case QEvent::GraphicsSceneMousePress:
            {
                if (hasDrawed)
                    return true;

                GraphicsSettings::instance()->setIsDrawing(true);
                break;
            }

            case QEvent::GraphicsSceneMouseMove:
            {
                if (hasDrawed)
                    return true;

                QLineF newLine(this->line().p1(), e->scenePos());
                setLine(newLine);
                break;
            }

            case QEvent::GraphicsSceneMouseRelease:
            {
                if (hasDrawed)
                    return true;

//                double x1 = line().p1().x();
//                double x2 = line().p2().x();
//                double y1 = line().p1().y();
//                double y2 = line().p2().y();
//                double dis = pow((pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0)), 0.5);

//                content = QString::number(dis*100/GraphicsSettings::instance()->getRatio(), 'f', 2).append("cm");

                GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
                scene->itemFinished(QString());

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



QString GraphicsLineItem::getDataString()
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

GraphicsLineItem *GraphicsLineItem::loadFromString(QString data)
{
    QString pos1_str = data.section(';', 0, 0);
    QString pos2_str = data.section(';', 1, 1);
    QPointF pos1, pos2;
    pos1.setX(pos1_str.section(',', 0, 0).toDouble() + Border);
    pos1.setY(pos1_str.section(',', 1, 1).toDouble() + Border);
    pos2.setX(pos2_str.section(',', 0, 0).toDouble() + Border);
    pos2.setY(pos2_str.section(',', 1, 1).toDouble() + Border);

    GraphicsLineItem *item = new GraphicsLineItem(QLineF(pos1, pos2));
    return item;
}


