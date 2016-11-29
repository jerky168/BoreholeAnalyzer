#pragma once

#include <QApplication>
#include <QColor>
#include <QFont>

class GraphicsSettings
{
public:
    static GraphicsSettings* instance();
    ~GraphicsSettings();

    void setPenWidth(int width);
    void setIsDrawing(bool isDrawing);
    void setFont(QFont);
    void setColor(QColor color);

    inline const QColor& getPenColor(){return penColor;}
    inline int getPenWidth(){return penWidth;}
    inline bool getIsDrawingFlag(){return isDrawingFlag;}
    inline QFont getFont() {return penFont;}

protected:
    GraphicsSettings();

private:
    int penWidth;
    bool isDrawingFlag;
    QFont penFont;
    QColor penColor;

    static GraphicsSettings* GraphicsSettingInstance;
};
