#pragma once

#include <QOpenGLWidget>

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = Q_NULLPTR);
    ~OpenGLWidget();
};


