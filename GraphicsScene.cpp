#include "GraphicsScene.h"


GraphicsScene::Mode GraphicsScene::curMode = GraphicsScene::MoveItem;
double GraphicsScene::ratio = 1.0;

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

    this->setSceneRect(0, 0, pixmap.width(), pixmap.height());
    this->addPixmap(pixmap);

    ratio = (qreal)(pixmap.height()) / (end - start);
    pixmap_start = start;
    pixmap_end = end;
}


// 绘制完item
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
    qreal x = mouseEvent->scenePos().x();
    qreal y = mouseEvent->scenePos().y();

    qreal height = pixmap_start + y / ratio;
    qreal degreed = 0;

    QString message = QString("height: ") + QString::number(height, 'f', 3) + "m\t";

    if (showInfo)
        emit showRealInfo(message);
    emit showStatus(message, 0);

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void GraphicsScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    painter->fillRect(rect, QBrush(Qt::lightGray));
    painter->setBrush(QBrush(Qt::white));
    painter->fillRect(QRect(sceneRect().x(), sceneRect().y(), width(), height()), QBrush(Qt::white));
}
