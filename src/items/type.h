#pragma once

#include <QGraphicsItem.h>

typedef enum
{
    Angle = 1,
    AnyShape,
    Ruler,
    Occurance,
    Rect,
    Text,
    customType =  QGraphicsItem::UserType
}ItemType;
