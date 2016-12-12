#pragma once

#include <QPen>
#include <QGraphicsSimpleTextItem>

#include "GraphicsScene.h"
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


    int type() const
    {
        return Text;
    }

    typedef struct
    {
        QPointF point;
        QString content;
    }Data;
    Data getData();


    QString getDataString();
    static GraphicsTextItem *loadFromString(QString data);

    void setFinished() {hasDrawed = true;}

protected:
    //bool sceneEvent(QEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool hasDrawed;

    void showTextDialog(QFont font);
    bool textDialogCloseFlag;
};
