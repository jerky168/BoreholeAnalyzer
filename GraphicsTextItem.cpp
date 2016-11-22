#include "GraphicsTextItem.h"

GraphicsTextItem::GraphicsTextItem(const QPointF& p, const QString& text, QGraphicsItem *parent)
    :QGraphicsSimpleTextItem(parent)
{
<<<<<<< 756f4e16bab91d7d8128f07f75bc6438a70bf51a

=======
//	textDialogCloseFlag = 0;
//	setPos(p);
//	setText(text);
//	setPen(QPen(GraphicsSettings::instance()->getPenColor(), 0));
//    setFlag(QGraphicsItem::ItemIsSelectable);
>>>>>>> 参考老版本的架构，继承Qt提供的基类，编写了不同功能的类，重写了view和scene的鼠标和滚轮事件。
}


GraphicsTextItem::~GraphicsTextItem()
{

}
