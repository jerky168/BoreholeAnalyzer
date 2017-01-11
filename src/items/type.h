#pragma once

#include <QGraphicsItem>

typedef enum
{
    NoType = 0,
    Angle,
    AnyShape,
    Ruler,
    Occurance,
    Rect,
    Text,
    customType =  QGraphicsItem::UserType
}ItemType;
