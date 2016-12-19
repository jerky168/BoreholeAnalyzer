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
    showInfo(false)
{

}

GraphicsScene::~GraphicsScene()
{

}


void GraphicsScene::setCurMode(Mode mode)
{
    curMode = mode;
    emit modeChanged(curMode);

    if (mode != MoveItem && item != Q_NULLPTR)
    {
        showInfo = true;
        item = Q_NULLPTR;
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



void GraphicsScene::updateIndexData(QPixmap pixmap, qreal start, qreal end, QVector<DefectWidget::ItemData>items)
{
    showInfo = true;

    clearScene();

    ratio = (qreal)(pixmap.height()) / (end - start);
    pixmap_start = start;
    pixmap_end = end;

    qreal realHeight = (qreal)pixmap.height() / (end - start);

    setSceneRect(0, 0, pixmap.width() + 2 * Border, realHeight + 2 * Border);
    QGraphicsPixmapItem *pixmapItem = addPixmap(pixmap);
    pixmapItem->setPos(Border, Border);


    for (int i = 0; i < items.count(); i++)
    {
        addItem(items.at(i).item);
    }


    this->update();
}




void GraphicsScene::itemFinished(QString content)
{
    showInfo = false;
    emit showRealInfo(content);

    item->ungrabMouse();
    curMode = MoveItem;
    emit modeChanged(curMode);

    emit itemInserted(item, QUuid::createUuid());
}


QImage GraphicsScene::getSceneImage()
{
    int width = this->sceneRect().width();
    int height = this->sceneRect().height();


    QImage image(width, height, QImage::Format_RGB32);
    QPainter painter(&image);
    this->render(&painter);


    return image;
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode == MoveItem && item != Q_NULLPTR)
    {
        showInfo = true;
        item = Q_NULLPTR;
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
                item->grabMouse();
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
                item = new GraphicsAnyshape(mouseEvent->scenePos());
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
    QFont font = GraphicsSettings::instance()->getFont();
    font.setPointSize(36);
    painter->setFont(font);


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

        painter->drawText(QPointF(x + Interval / 2, y + 2 * Segment), QString::number(pixmap_start + i * 0.1, 'f', 1));
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
                painter->drawText(QPointF(x+Segment / 2, y + 2*Segment), "N");
                break;
            case 1:
                painter->drawText(QPointF(x+Segment / 2, y + 2*Segment), "E");
                break;
            case 2:
                painter->drawText(QPointF(x+Segment / 2, y + 2*Segment), "S");
                break;
            case 3:
                painter->drawText(QPointF(x+Segment / 2, y + 2*Segment), "W");
                break;
            case 4:
                painter->drawText(QPointF(x+Segment / 2, y + 2*Segment), "N");
                break;
            default:
                break;
        }
    }
    painter->drawLines(lines);

}



QImage GraphicsScene::getImageFromData(QPixmap pixmap, qreal start, qreal end, QVector<DefectWidget::ItemData> items)
{
    GraphicsScene *scene = new GraphicsScene;
    scene->updateIndexData(pixmap, start, end, items);
    QImage image = scene->getSceneImage();
    delete scene;
    return image;
}


