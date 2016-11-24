#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
{
    penFont = QApplication::font();
    penFont.setPointSize(30);
    penColor = QColor(Qt::black);
    penWidth = 8;
    isDrawingFlag = false;
}

GraphicsSettings::~GraphicsSettings()
{
//    if (GraphicsSettingInstance != Q_NULLPTR)
//        delete GraphicsSettingInstance;
//    GraphicsSettingInstance = Q_NULLPTR;
}

//GraphicsSettings* GraphicsSettings::instance()
//{
//    if (GraphicsSettingInstance == Q_NULLPTR)
//        GraphicsSettingInstance = new GraphicsSettings();
//    return GraphicsSettingInstance;
//}


void GraphicsSettings::setColor(QColor color)
{
    penColor = color;
}

void GraphicsSettings::setPenWidth(quint8 width)
{
    penWidth = width;
}

void GraphicsSettings::setFont(QFont font)
{
    penFont = font;
}

void GraphicsSettings::setIsDrawing(bool flag)
{
    isDrawingFlag = flag;
}


