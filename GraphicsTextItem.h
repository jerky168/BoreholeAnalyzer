#pragma once

#include <QPen>
#include <QGraphicsSimpleTextItem>

#include "GraphicsSettings.h"
#include "TextDialog.h"

#include "stable.h"
#include "type.h"

class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
    GraphicsTextItem(const QPointF& position, const QString& text = QString(), QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsTextItem();

    bool getTextDialogCloseFlag();

    typedef struct
    {
        QPointF point;
        QString content;
    }Data;

    int type() const
    {
        return Text;
    }
    Data getData();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
    bool sceneEvent(QEvent *event);

private:
    void showTextDialog(QFont font);
    bool textDialogCloseFlag;
};
