#pragma once

#include <QPen>
#include <QApplication>
#include <QGraphicsSimpleTextItem>

#include "TextDialog.h"


class GraphicsTextItem : public QGraphicsSimpleTextItem
{
public:
    GraphicsTextItem(const QPointF& position, const QString& text = QString(), QGraphicsItem *parent = Q_NULLPTR);
    ~GraphicsTextItem();

private:
    bool textDialogCloseFlag;
    void showTextDialog(QFont font);
};
