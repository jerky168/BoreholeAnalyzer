#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_xRot(0),
    m_yRot(0),
    m_zRot(0)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));

    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::~OpenGLWidget()
{
    cleanup();
}

QSize OpenGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OpenGLWidget::sizeHint() const
{
    return QSize(400, 400);
}



static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void OpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot)
    {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void OpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot)
    {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void OpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot)
    {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}


void OpenGLWidget::updateImage(const QImage &image)
{

}


void OpenGLWidget::cleanup()
{

}


// 设置范围
void OpenGLWidget::setRollRange(int start, int end)
{
    rollStart = start;
    rollEnd = end;
}



// Sets up the OpenGL resources and state. Gets called once before the first time resizeGL() or paintGL() is called.
void OpenGLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWidget::cleanup);
}

// Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
void OpenGLWidget::paintGL()
{

}


// Sets up the OpenGL viewport, projection, etc.
// Gets called whenever the widget has been resized (and also when it is shown for the first time because all newly created widgets get a resize event automatically).
void OpenGLWidget::resizeGL(int width, int height)
{

}



void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();

}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{

}


void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{

}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{

}


void OpenGLWidget::loadGLTextures()
{
    if (loadImage.isNull())
        return;
}


