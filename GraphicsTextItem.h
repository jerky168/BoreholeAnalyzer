#pragma once

#include <QPen>
#include <QGraphicsSimpleTextItem>

#include "GraphicsSettings.h"
#include "TextDialog.h"


class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
    GraphicsTextItem(const QPointF& position, const QString& text = QString(), QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsTextItem();

    bool getTextDialogCloseFlag();

    //ItemType getType() {return TextBox;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
    bool sceneEvent(QEvent *event);

private:
    void showTextDialog(QFont font);
    bool textDialogCloseFlag;
};
