#include "GraphicsScene.h"


/* 本类负责item的管理和编辑模式的管理
 * item的管理：item的增删修改
 * 编辑模式的管理：根据鼠标在scene中的动作以及工具栏的action的动作 来决定模式的改变。
 * */

/******************************** public functions **************************************/

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    curMode(Normal),
    item(Q_NULLPTR)
{
    QPixmap pixmap;
    pixmapItem = this->addPixmap(pixmap);
}

GraphicsScene::~GraphicsScene()
{

}


// 重置item指针
void GraphicsScene::initItem()
{
    if (item != Q_NULLPTR)
    {
        item->ungrabMouse();
        item = Q_NULLPTR;
    }
}

// 更改模式
void GraphicsScene::setCurMode(Mode mode)
{
    emit modeChanged(curMode, mode);
    curMode = mode;
}

/******************************** public slots **************************************/
// 更新图片
void GraphicsScene::updatePixmap(QPixmap pixmap)
{
    pixmapItem->setPixmap(pixmap);
}

// 清除图片
void GraphicsScene::clearPixmap()
{
    QPixmap pixmap;
    pixmapItem->setPixmap(pixmap);
}




// 根据当前模式创建item
QGraphicsItem *GraphicsScene::createNewItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (curMode)
    {
    case InsertTextBox:
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
        initItem();
        emit modeChanged(curMode, Normal);
        curMode = Normal;
    }
    else if (curMode != Normal)
    {
        item = createNewItem(mouseEvent);
        this->addItem(item);
        emit itemInserted(item);
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
    // 如果右键释放 退出编辑模式
//    if (mouseEvent->button() == Qt::RightButton)
//    {
//        if (curMode != Normal)
//        {
//            emit itemInserted(item);
//            initItem();
//        }
//    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
