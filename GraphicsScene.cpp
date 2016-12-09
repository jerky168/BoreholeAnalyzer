#include "GraphicsScene.h"


GraphicsScene::Mode GraphicsScene::curMode = GraphicsScene::MoveItem;
qreal GraphicsScene::ratio = 1.0;

/* 本类负责item的管理和编辑模式的管理
 * item的管理：item的增删修改
 * 编辑模式的管理：根据鼠标在scene中的动作以及工具栏的action的动作 来决定模式的改变。
 * */

/******************************** public functions **************************************/
GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    item(Q_NULLPTR),
    pixmapRect(QRectF()),
    pixmap_start(0.0),
    pixmap_end(0.0),
    showInfo(true),
    hasSaved(false)
{


}

GraphicsScene::~GraphicsScene()
{

}


void GraphicsScene::setCurMode(Mode mode)
{
    curMode = mode;
    emit modeChanged(curMode);

    if (mode != MoveItem && item != Q_NULLPTR && !hasSaved)
    {
        deleteLastItem();
    }

}


void GraphicsScene::clearScene()
{
    clear();
    ratio = 0.0;
    pixmapRect = QRectF();
    pixmap_start = 0.0;
    pixmap_end = 0.0;
}


void GraphicsScene::updatePixmap(QPixmap pixmap, qreal start, qreal end)
{
    clearScene();

    ratio = (qreal)(pixmap.height()) / (end - start);
    pixmap_start = start;
    pixmap_end = end;

    qreal realHeight = (qreal)pixmap.height() / (end - start);

    setSceneRect(0, 0, pixmap.width() + 2 * Border, realHeight + 2 * Border);
    QGraphicsPixmapItem *pixmapItem = addPixmap(pixmap);
    pixmapItem->setPos(Border, Border);
}



void GraphicsScene::itemFinished(QString content)
{
    showInfo = false;
    emit showRealInfo(content);

    item->ungrabMouse();
    curMode = MoveItem;
    emit modeChanged(curMode);
}


void GraphicsScene::saveLastItem()
{
    hasSaved = false;
    showInfo = true;
    emit itemSaved(item);
    item = Q_NULLPTR;
}


void GraphicsScene::deleteLastItem()
{
    hasSaved = false;
    showInfo = true;
    delete item;
    item = Q_NULLPTR;
}


void GraphicsScene::getSavedItem(QVector<QGraphicsItem *>items)
{

}



void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode == MoveItem && item != Q_NULLPTR && !hasSaved)
    {
        deleteLastItem();
    }

    if (item != Q_NULLPTR)
    {

    }
    else if (curMode != MoveItem && (mouseEvent->buttons() & Qt::RightButton))
    {
        curMode = MoveItem;
        emit modeChanged(curMode);
    }
    else if (curMode != MoveItem && (mouseEvent->buttons() & Qt::LeftButton))
    {
        switch (curMode)
        {
            case InsertTextBox :
            {
                item = new GraphicsTextItem(mouseEvent->scenePos());
                addItem(item);
                item = Q_NULLPTR;
                curMode = MoveItem;
                emit modeChanged(curMode);
                break;
            }
            case InsertLine :
            {
                item = new GraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }
            case InsertShift :
            {
                item = new GraphicsAngleItem(mouseEvent->scenePos());
                addItem(item);
                item->grabMouse();
                break;
            }
            case InsertRectangle :
            {
                item = new GraphicsRectItem(QRectF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }

            case InsertAnyShape :
            {
                GraphicsAnyshape *realItem = new GraphicsAnyshape(mouseEvent->scenePos());
                item = realItem;
                addItem(item);
                item->grabMouse();
                break;
            }

            case InsertOccurance :
            {
                item = new GraphicsOccurance(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                addItem(item);
                item->grabMouse();
                break;
            }

            default:
            {
                break;
            }
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}


void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qreal x = mouseEvent->scenePos().x() - Border;
    qreal y = mouseEvent->scenePos().y() - Border;

    qreal height = pixmap_start + y / ratio;
    qreal degreed = 0;

    QString message = QString("height: ") + QString::number(height, 'f', 3) + "m\t";

    if (showInfo)
        emit showRealInfo(message);
    emit showStatus(message, 0);

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QBrush(Qt::lightGray));
    painter->setBrush(QBrush(Qt::white));


    if (items().isEmpty())
        return;


    QPointF topLeft = QPointF(sceneRect().x() + Border, sceneRect().y() + Border);
    QPointF bottomRight = QPointF(width() - Border, height() - Border);

    painter->fillRect(QRectF(topLeft, bottomRight), QBrush(Qt::white));


    QPen thisPen(Qt::black);
    thisPen.setWidth(8);

    painter->setPen(thisPen);
    painter->setFont(GraphicsSettings::instance()->getFont());


    QVector<QLineF> lines;
    QLineF line;

    line.setLine(sceneRect().x() + Interval, Border, sceneRect().x() + Interval, height() - Border);
    lines << line;
    line.setLine(Border, Interval, width() - Border, Interval);
    lines << line;


    for (int i = 0; i < 11; i++)
    {
        qreal x = sceneRect().x() + Interval;
        qreal y = Border + (height() - 2 * Border) / 10 * i;
        line.setLine(x, y , x + Segment, y);
        lines << line;

        painter->drawText(QPointF(x + Segment, y + 2 * Segment), QString::number(pixmap_start + i * 0.1, 'f', 1));
    }

    for (int i = 0; i < 5; i++)
    {
        qreal x = Border + (width() - 2 * Border) / 4 * i;
        qreal y = sceneRect().y() + Interval;
        line.setLine(x, y , x, y + Segment);
        lines << line;

        switch(i)
        {
            case 0:
                painter->drawText(QPointF(x+Segment, y + 2*Segment), "N");
                break;
            case 1:
                painter->drawText(QPointF(x+Segment, y + 2*Segment), "E");
                break;
            case 2:
                painter->drawText(QPointF(x+Segment, y + 2*Segment), "S");
                break;
            case 3:
                painter->drawText(QPointF(x+Segment, y + 2*Segment), "W");
                break;
            case 4:
                painter->drawText(QPointF(x+Segment, y + 2*Segment), "N");
                break;
            default:
                break;
        }
    }
    painter->drawLines(lines);

}
