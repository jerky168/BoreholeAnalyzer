#pragma once

#include <QGLWidget>
#include <QOpenGLFunctions>

#include <QDesktopWidget>

#include <QtMath>
#include <QTimer>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#ifdef Q_OS_WIN
#include <GL/glu.h>
#endif

#ifdef Q_OS_OSX
#include <OpenGL/glu.h>
#endif

class Widget3D : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Widget3D(QWidget *parent, bool fs = false);
    ~Widget3D();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setImage(QImage image, qreal start, qreal end);

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void handleZoomIn();
    void handleZoomOut();
    void handleLeftSpin();
    void handleRightSpin();

    void startLeftSpin();
    void startRightSpin();
    void stopAutoSpin();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QTimer timerLeft, timerRight;
    QImage loadImage;

    bool fullScreen, altDown, light;
    qreal rollStart, rollEnd;

    QPoint lastPos;
    GLfloat xRot, yRot, zRot;
    GLfloat xSpeed, ySpeed;
    GLfloat x, y, z;

    GLUquadricObj *quadratic;

    void setParameters(int width, int height);

signals:
    void clicked();
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

};
