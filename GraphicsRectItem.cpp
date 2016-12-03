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
    qDebug() << "item press event";
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

    qDebug() << "item move event";
}

void GraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!hasDrawed)
    {
        qreal x = rect().x() + rect().width() / 2;
        qreal y = rect().y() + rect().height() / 2;
        qreal area = rect().width() * rect().height() / qPow(GraphicsScene::getRatio(), 2);
        QGraphicsSimpleTextItem *textItem = scene()->addSimpleText(QString::number(area, 'f', 2).append("m2"), QFont("Times", 40, QFont::Bold));
        textItem->setParentItem(this);
        textItem->setPos(x, y);
    }

    hasDrawed = true;
    qDebug() << "item release event";
}


