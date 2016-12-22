#pragma once

#include <QGraphicsItem.h>

typedef enum
{
    customType =  QGraphicsItem::UserType,
    Angle,
    AnyShape,
    Ruler,
    Occurance,
    Rect,
    Text
}ItemType;
