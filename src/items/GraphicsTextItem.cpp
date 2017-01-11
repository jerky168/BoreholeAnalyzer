#include "GraphicsTextItem.h"

GraphicsTextItem::GraphicsTextItem(const QPointF &position, const QString &text, QGraphicsItem *parent) :
    QGraphicsSimpleTextItem(parent),
    hasDrawed(false),
    currentColor(TextInputDialog::Black)
{
    setPos(position);
    setText(text);
    setBrush(QBrush(Qt::black));
    setFont(GraphicsSettings::instance()->getFont());
    QFont font = this->font();
    font.setBold(true);
    setFont(font);
}

GraphicsTextItem::~GraphicsTextItem()
{

}

void GraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (hasDrawed)
        return;

    setPos(event->scenePos());

    TextInputDialog *inputDialog = new TextInputDialog(text(), font().pointSize(), currentColor);
    if (QDialog::Accepted == inputDialog->exec())
    {
        setText(inputDialog->getText());
        QFont font = this->font();
        font.setPointSize(inputDialog->getFontSize());
        setFont(font);
        GraphicsSettings::instance()->setFont(font);

        currentColor = inputDialog->getColor();
        switch (currentColor) {
        case TextInputDialog::Black:
            setBrush(QBrush(Qt::black));
            break;
        case TextInputDialog::White:
            setBrush(QBrush(Qt::white));
            break;
        case TextInputDialog::Red:
            setBrush(QBrush(Qt::red));
            break;
        case TextInputDialog::Green:
            setBrush(QBrush(Qt::green));
            break;
        case TextInputDialog::Blue:
            setBrush(QBrush(Qt::blue));
            break;
        case TextInputDialog::Yellow:
            setBrush(QBrush(Qt::yellow));
            break;
        default:
            break;
        }

        delete inputDialog;
        hasDrawed = true;
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        scene->itemFinished(text());
    }
    else
    {
        delete inputDialog;
        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        scene->itemAborted();
    }
}

void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    TextInputDialog *inputDialog = new TextInputDialog(text(), font().pointSize(), currentColor);
    if (QDialog::Accepted == inputDialog->exec())
    {
        setText(inputDialog->getText());
        QFont font = this->font();
        font.setPointSize(inputDialog->getFontSize());
        setFont(font);
        GraphicsSettings::instance()->setFont(font);

        currentColor = inputDialog->getColor();
        switch (currentColor) {
        case TextInputDialog::Black:
            setBrush(QBrush(Qt::black));
            break;
        case TextInputDialog::White:
            setBrush(QBrush(Qt::white));
            break;
        case TextInputDialog::Red:
            setBrush(QBrush(Qt::red));
            break;
        case TextInputDialog::Green:
            setBrush(QBrush(Qt::green));
            break;
        case TextInputDialog::Blue:
            setBrush(QBrush(Qt::blue));
            break;
        case TextInputDialog::Yellow:
            setBrush(QBrush(Qt::yellow));
            break;
        default:
            break;
        }

        delete inputDialog;

        GraphicsScene *scene = dynamic_cast<GraphicsScene *>(this->scene());
        scene->updateTable();
    }
}




QString GraphicsTextItem::getDataString()
{
   QString data;
   data.append(QString::number(scenePos().x() - Border, 'f', 2));
   data.append(",");
   data.append(QString::number(scenePos().y() - Border, 'f', 2));
   data.append(";");
   data.append(text());

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

    return item;
}


