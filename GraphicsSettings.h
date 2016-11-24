#pragma once

#include <QApplication>
#include <QColor>

class GraphicsSettings
{
public:
    GraphicsSettings();
    ~GraphicsSettings();
    //static GraphicsSettings* instance();

    void setColor(QColor color);
    void setPenWidth(quint8 width);
    void setFont(QFont font);
    void setIsDrawing(bool flag);

    inline const QColor& getPenColor(){return penColor;}
    inline int getPenWidth(){return penWidth;}
    inline bool getIsDrawingFlag(){return isDrawingFlag;}
    inline QFont getFont() {return penFont;}

private:
    //static GraphicsSettings* GraphicsSettingInstance;
    QColor penColor;
    quint8 penWidth;
    QFont penFont;
    bool isDrawingFlag;
};
