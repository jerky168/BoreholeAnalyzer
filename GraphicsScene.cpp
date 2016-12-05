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

// 当item绘制完后
void GraphicsScene::itemInserted()
{
    item->ungrabMouse();
    item = Q_NULLPTR;
    curMode = MoveItem;
    emit modeChanged(curMode);
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (item != Q_NULLPTR)
        return;

    if (curMode == MoveItem)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }

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
        case InsertAnyShape :
        {
            item = new GraphicsAnyshape(mouseEvent->scenePos());
            break;
        }
        default:
        {
            break;
        }
    }
    this->addItem(item);
    item->grabMouse();
}



void GraphicsScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    painter->fillRect(rect, QBrush(Qt::lightGray));
    painter->setBrush(QBrush(Qt::white));
    painter->fillRect(QRect(sceneRect().x(), sceneRect().y(), width(), height()), QBrush(Qt::white));
}
