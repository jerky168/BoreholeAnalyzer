#include "GraphicsSettings.h"

GraphicsSettings* GraphicsSettings::GraphicsSettingInstance = Q_NULLPTR;

GraphicsSettings* GraphicsSettings::instance()
{
    if (GraphicsSettingInstance == Q_NULLPTR)
    {
        GraphicsSettingInstance = new GraphicsSettings;
    }
    return GraphicsSettingInstance;
}

GraphicsSettings::~GraphicsSettings()
{
    if (GraphicsSettingInstance != Q_NULLPTR)
    {
        delete GraphicsSettingInstance;
        GraphicsSettingInstance = Q_NULLPTR;
    }
}

GraphicsSettings::GraphicsSettings() :
    penWidth(8),
    isDrawingFlag(false),
    penFont(QApplication::font()),
    penColor(Qt::black),
    imageXRatio(1.0),
    imageYRatio(1.0)
{
    penFont.setPointSize(50);
}

void GraphicsSettings::setPenWidth(int width)
{
    penWidth = width;
}

void GraphicsSettings::setIsDrawing(bool isDrawing)
{
    isDrawingFlag = isDrawing;
}

void GraphicsSettings::setFont(QFont font)
{
    penFont = font;
}

void GraphicsSettings::setColor(QColor color)
{
    penColor = color;
}

void GraphicsSettings::setRatio(qreal xRatio, qreal yRatio)
{
    imageXRatio = xRatio;
    imageYRatio = yRatio;
}

