#include "GraphicsScene.h"


/* 本类负责item的管理和编辑模式的管理
 * item的管理：item的增删修改
 * 编辑模式的管理：根据鼠标在scene中的动作以及工具栏的action的动作 来决定模式的改变。
 * */

/******************************** public functions **************************************/
GraphicsScene::GraphicsScene(DbHandler *dbHandler, QObject *parent) :
    QGraphicsScene(parent),
    curMode(MoveItem),
    item(Q_NULLPTR)
{
    QPixmap pixmap;
    pixmapItem = this->addPixmap(pixmap);

    handler = dbHandler;

    // clear item group
    itemGroup.clear();
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
    pixmapItem->setPixmap(pixmap);

    // clear all items
    for (int i = 0; i < itemGroup.count(); i++)
    {
        removeItem(itemGroup[i]);
    }
    itemGroup.clear();
}

void GraphicsScene::clearScene()
{
    // clear pixmap
    QPixmap pixmap;
    pixmapItem->setPixmap(pixmap);

    // clear all items
    for (int i = 0; i < itemGroup.count(); i++)
    {
        removeItem(itemGroup[i]);
    }
    itemGroup.clear();
}



// 根据当前模式创建item
QGraphicsItem *GraphicsScene::createNewItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (curMode)
    {
        // 添加文本框
        case InsertTextBox:
        {
            item = new GraphicsTextItem(mouseEvent->scenePos());
            itemGroup.append(item);
            break;
        }

        // 插入直线
        case InsertLine:
        {
            item = new GraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            itemGroup.append(item);
            break;
        }

        case InsertShift:
        {
            item = new GraphicsAngleItem(mouseEvent->scenePos());
            itemGroup.append(item);
            break;
        }

        default:
            break;
        }
    return item;
}


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode != MoveItem && item == Q_NULLPTR)
    {
        item = createNewItem(mouseEvent);
        addItem(item);
    }
    else
    {
        curMode = MoveItem;
        emit modeChanged(curMode);
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
        emit modeChanged(curMode);

        initItem();
    }
}


void GraphicsScene::clearItemVector()
{

}
