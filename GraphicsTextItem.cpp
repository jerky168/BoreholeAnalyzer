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

void GraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    event = Q_NULLPTR;
    setFlag(QGraphicsItem::ItemIsMovable, false);
}

void GraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

    GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
    scene->itemFinished(text());

    //loadFromString(getDataString());

}


GraphicsTextItem::Data GraphicsTextItem::getData()
{
    Data data;
    data.point = this->scenePos();
    data.content = this->text();
    return data;
}



QString GraphicsTextItem::getDataString()
{
   QString data;
   data.append(QString::number(scenePos().x() - Border, 'f', 2));
   data.append(",");
   data.append(QString::number(scenePos().y() - Border, 'f', 2));
   data.append(";");
   data.append(text());

   qDebug() << scenePos() << text();
   return data;
}

GraphicsTextItem * GraphicsTextItem::loadFromString(QString data)
{
    QString pos_str = data.section(';', 0, 0);
    QString text_str = data.section(';', 1, 1);
    QPointF pos;
    pos.setX(pos_str.section(',', 0, 0).toDouble() + Border);
    pos.setY(pos_str.section(',', 1, 1).toDouble() + Border);

    GraphicsTextItem *item = new GraphicsTextItem(pos, text_str);

    qDebug() << item->scenePos() << item->text();
    return item;
}

