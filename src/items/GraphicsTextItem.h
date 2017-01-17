#pragma once

#include <QPen>
#include <QGraphicsSimpleTextItem>

#include "GraphicsScene.h"
#include "GraphicsSettings.h"
#include "TextInputDialog.h"
#include "type.h"

class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
    GraphicsTextItem(const QPointF& position, const QString& text = QString(), QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsTextItem();

    int type() const
    {
        return Text;
    }

    QString getDataString();
    static GraphicsTextItem *loadFromString(QString data);
    void setFinished() {hasDrawed = true;}


    QString getRemark() {return remarkContent;}
    void setRemark(QString remark) {remarkContent = remark;}

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;
    TextInputDialog::Color currentColor;
    QString remarkContent;

};
