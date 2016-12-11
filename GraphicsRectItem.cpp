#include "GraphicsRectItem.h"

GraphicsRectItem::GraphicsRectItem(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent),
    hasDrawed(false),
    origPos(rect.topLeft())
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
    thisPen.setColor(Qt::yellow);
    thisPen.setWidth(pen().width());
    thisPen.setStyle(Qt::SolidLine);
    painter->setPen(thisPen);

    painter->drawRect(rect());
}


void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}


void GraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!hasDrawed)
    {
        qreal x = (origPos.x() > event->pos().x() ? event->pos().x() : origPos.x());
        qreal y = (origPos.y() > event->pos().y() ? event->pos().y() : origPos.y());
        qreal w = qFabs(origPos.x() - event->pos().x());
        qreal h = qFabs(origPos.y() - event->pos().y());

        QRectF newRect(x, y, w, h);
        setRect(newRect);
    }
}

void GraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!hasDrawed)
    {
        qreal x = rect().x() + rect().width() / 2;
        qreal y = rect().y() + rect().height() / 2;
        qreal area = rect().width() * rect().height() / qPow(GraphicsScene::getRatio(), 2) * 10000;

        content = QString::number(area, 'f', 2).append("cm2");

        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        scene->itemFinished(content);

        loadFromString(getDataString());

    }

    hasDrawed = true;
}

GraphicsRectItem::Data GraphicsRectItem::getData()
{
    Data data;
    data.points[0] = this->rect().topLeft();
    data.points[1] = this->rect().bottomRight();
    return data;
}


QString GraphicsRectItem::getDataString()
{
    QString data;
    data.append(QString::number(rect().left() - Border, 'f', 2));
    data.append(",");
    data.append(QString::number(rect().top() - Border, 'f', 2));
    data.append(";");
    data.append(QString::number(rect().right() - Border, 'f', 2));
    data.append(",");
    data.append(QString::number(rect().bottom() - Border, 'f', 2));
    qDebug() << rect();
    return data;
}

GraphicsRectItem * GraphicsRectItem::loadFromString(QString data)
{
    QString pos1_str = data.section(';', 0, 0);
    QString pos2_str = data.section(';', 1, 1);
    QPointF pos1, pos2;
    pos1.setX(pos1_str.section(',', 0, 0).toDouble() + Border);
    pos1.setY(pos1_str.section(',', 1, 1).toDouble() + Border);
    pos2.setX(pos2_str.section(',', 0, 0).toDouble() + Border);
    pos2.setY(pos2_str.section(',', 1, 1).toDouble() + Border);

    GraphicsRectItem *item = new GraphicsRectItem(QRectF(pos1, pos2));
    qDebug() << item->rect();

    return item;
}

