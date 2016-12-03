#include "GraphicsScene.h"


double GraphicsScene::ratio = 1.0;
GraphicsScene::Mode GraphicsScene::curMode = GraphicsScene::MoveItem;

/* 本类负责item的管理和编辑模式的管理
 * item的管理：item的增删修改
 * 编辑模式的管理：根据鼠标在scene中的动作以及工具栏的action的动作 来决定模式的改变。
 * */

/******************************** public functions **************************************/
GraphicsScene::GraphicsScene(DbHandler *dbHandler, QObject *parent) :
    QGraphicsScene(parent),
    item(Q_NULLPTR)
{
    handler = dbHandler;


}

GraphicsScene::~GraphicsScene()
{

}


// 重置item指针
void GraphicsScene::initItem()
{
    if (item != Q_NULLPTR)
    {
        item = Q_NULLPTR;
    }
}

// 更改模式
void GraphicsScene::setCurMode(Mode mode)
{
    curMode = mode;
    emit modeChanged(curMode);
}

/******************************** public slots **************************************/
void GraphicsScene::updatePixmap(QPixmap pixmap)
{
    clear();
    this->setSceneRect(0, 0, pixmap.width(), pixmap.height());
    this->addPixmap(pixmap);
    ratio = (double)pixmap.height();
}


QGraphicsItem *GraphicsScene::createNewItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (curMode)
    {
        case InsertTextBox :
        {
            item = new GraphicsTextItem(mouseEvent->scenePos());
            break;
        }

        case InsertLine :
        {
            item = new GraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            break;
        }

        case InsertShift :
        {
            item = new GraphicsAngleItem(mouseEvent->scenePos());
            break;
        }

        case InsertRectangle :
        {
            item = new GraphicsRectItem(QRectF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            break;
        }

        default:
        {
            break;
        }

    }
    return item;
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode != MoveItem && item == Q_NULLPTR)
    {
        item = createNewItem(mouseEvent);
        addItem(item);
        item->grabMouse();
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    if (curMode != MoveItem)
    {
        curMode = MoveItem;
        item->ungrabMouse();
        emit modeChanged(curMode);

        initItem();
    }
}


void GraphicsScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    painter->fillRect(rect, QBrush(Qt::lightGray));
    painter->setBrush(QBrush(Qt::white));
    painter->fillRect(QRect(sceneRect().x(), sceneRect().y(), width(), height()), QBrush(Qt::white));
}
