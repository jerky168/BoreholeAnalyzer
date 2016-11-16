#include "GraphicsTextItem.h"



GraphicsTextItem::GraphicsTextItem( const QPointF& p, const QString& text, QGraphicsItem *parent )
	:QGraphicsSimpleTextItem(0, parent)
{
	textDialogCloseFlag = 0;
	setPos(p);
	setText(text);
	setPen(QPen(GraphicsSettings::instance()->getPenColor(), 0));
	setFlag(QGraphicsItem::ItemIsSelectable);

	showTextDialog(GraphicsSettings::instance()->getFont());
}


GraphicsTextItem::~GraphicsTextItem()
{

}
