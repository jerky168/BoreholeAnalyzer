#include "GraphicsTextItem.h"

GraphicsTextItem::GraphicsTextItem(const QPointF& position, const QString& text, QGraphicsItem *parent) :
    QGraphicsSimpleTextItem(parent),
    textDialogCloseFlag(false)
{
    this->setPos(position);
    this->setText(text);
    this->setPen(QPen(Qt::black));
    this->setFlag(QGraphicsItem::ItemIsMovable);

    this->showTextDialog(QApplication::font());
}

GraphicsTextItem::~GraphicsTextItem()
{

}


void GraphicsTextItem::showTextDialog(QFont f)
{
    TextDialog* dialog = new TextDialog(f);
    dialog->setText(text());
    dialog->setWindowTitle(QObject::tr("Input text"));
    dialog->exec();
    if (1 == dialog->getCloseFlag())
    {
        textDialogCloseFlag = 1;
    }
    if (!dialog->getText().isEmpty())
    {
        this->setText(dialog->getText());
    }
    this->setFont(dialog->getFont());
    delete dialog;
}



