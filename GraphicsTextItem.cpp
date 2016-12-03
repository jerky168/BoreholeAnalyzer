#include "GraphicsTextItem.h"

GraphicsTextItem::GraphicsTextItem(const QPointF& position, const QString& text, QGraphicsItem *parent) :
    QGraphicsSimpleTextItem(parent),
    textDialogCloseFlag(false)
{
    setPos(position);
    setText(text);
    setPen(QPen(GraphicsSettings::instance()->getPenColor(), 0));
    setFlag(QGraphicsItem::ItemIsMovable);

    showTextDialog(GraphicsSettings::instance()->getFont());
}


GraphicsTextItem::~GraphicsTextItem()
{

}

bool GraphicsTextItem::getTextDialogCloseFlag()
{
    return textDialogCloseFlag;
}

void GraphicsTextItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
    event = Q_NULLPTR;
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void GraphicsTextItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    event = Q_NULLPTR;
    if (isSelected())
    {
        setFlag(QGraphicsItem::ItemIsMovable);
    }
    else
    {
        GraphicsSettings::instance()->setIsDrawing(true);
    }
}
void GraphicsTextItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    QGraphicsItem::mouseReleaseEvent(event);
}


bool GraphicsTextItem::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMouseDoubleClick)
    {
        GraphicsSettings::instance()->setFont(font());
        showTextDialog(GraphicsSettings::instance()->getFont());
    }
    else
    {
        QGraphicsItem::sceneEvent(event);
    }
    return true;
}

void GraphicsTextItem::showTextDialog(QFont font)
{
    TextDialog *dialog = new TextDialog(font);
    dialog->setText(this->text());
    dialog->setWindowTitle("输入文本");
    dialog->exec();

    if (dialog->getCloseFlag())
    {
        textDialogCloseFlag = true;
    }

    if (!dialog->getText().isEmpty())
    {
        this->setText(dialog->getText());
    }
    this->setFont(dialog->getFont());
    delete dialog;
}



