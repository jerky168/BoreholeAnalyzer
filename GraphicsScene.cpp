#include "GraphicsScene.h"


/* 本类负责item的管理和编辑模式的管理
 * item的管理：item的增删修改
 * 编辑模式的管理：根据鼠标在scene中的动作以及工具栏的action的动作 来决定模式的改变。
 * */

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    curMode(MoveItem),
    item(Q_NULLPTR)
{
    QPixmap pixmap;
    pixmapItem = this->addPixmap(pixmap);
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::updatePixmap(QPixmap pixmap)
{
    pixmapItem->setPixmap(pixmap);
}

void GraphicsScene::clearPixmap()
{
    QPixmap pixmap;
    pixmapItem->setPixmap(pixmap);
}

// 根据当前模式创建
QGraphicsItem *GraphicsScene::createNewItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (curMode)
    {
    case InsertText:
    {
        item = new GraphicsTextItem(mouseEvent->scenePos());
        emit itemInserted(item);
        break;
    }
    default:
        break;
    }
    return item;
}


// 鼠标点击事件
void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        emit modeChanged(curMode, MoveItem);
        curMode = MoveItem;
        return;
    }
    else if (curMode != MoveItem && item == Q_NULLPTR)
    {
        item = createNewItem(mouseEvent);
        this->addItem(item);
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

// 鼠标移动事件
void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{


    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

//　鼠标释放事件
void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{


    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
