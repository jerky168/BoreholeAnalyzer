#include "GraphicsScene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    curMode(Normal)
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


void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {

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
}
