#pragma once

#include <QToolBar>
#include <QComboBox>
#include <QPainter>
#include <QSpinBox>
#include <QFontComboBox>

class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    ToolBar(QWidget *parent = Q_NULLPTR);
    ~ToolBar();

private:
    QComboBox *colorCBox;
    QSpinBox *lineSizeSBox;
};
