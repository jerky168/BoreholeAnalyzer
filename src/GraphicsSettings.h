#pragma once

#include <QApplication>
#include <QColor>
#include <QFont>

#include "GraphicsScene.h"


class GraphicsSettings
{
public:
    static GraphicsSettings *instance();
    ~GraphicsSettings();


    void setPenWidth(int width);
    void setIsDrawing(bool isDrawing);
    void setFont(QFont);
    void setColor(QColor color);
    void setRatio(qreal xRatio, qreal yRatio);

    inline const QColor& getPenColor(){return penColor;}
    inline int getPenWidth(){return penWidth;}
    inline bool getIsDrawingFlag(){return isDrawingFlag;}
    inline QFont getFont() {return penFont;}
    inline qreal getXRatio() {return imageXRatio;}
    inline qreal getYRatio() {return imageYRatio;}

protected:
    GraphicsSettings();

private:
    int penWidth;
    bool isDrawingFlag;
    QFont penFont;
    QColor penColor;

    qreal imageXRatio, imageYRatio;


    static GraphicsSettings* GraphicsSettingInstance;
};
