#include "Widget3D.h"
#include <QMessageBox>
#include <QDebug>

//Ambient Light是模拟漫反射的一种光源。它能将灯光均匀地照射在场景中每个物体上面，
//在使用Ambient Light时可以忽略方向和角度，只考虑光源的位置。
GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };

//Diffuse Light 检索或设置光线发出的漫射色。
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };

//光源位置
GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };


Widget3D::Widget3D(QWidget *parent, bool fs) :
    QGLWidget(parent),
    fullScreen(fs),
    altDown(false),
    light(false),
    x(0),
    y(0),
    z(0),
    xRot(0),
    yRot(0),
    zRot(0),
    rollStart(0.0),
    rollEnd(1.0)
{
    setMinimumSize(600, 800);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_DeleteOnClose);

    timerLeft.setInterval(100);
    timerRight.setInterval(100);

    connect(&timerLeft, &QTimer::timeout, [this]() {setZRotation(zRot - 20);});
    connect(&timerRight, &QTimer::timeout, [this]() {setZRotation(zRot + 20);});

    makeCurrent();
    initializeGL();
}

Widget3D::~Widget3D()
{
    gluDeleteQuadric(quadratic);
}

QSize Widget3D::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Widget3D::sizeHint() const
{
    return QSize(200, 200);
}

void Widget3D::setImage(QImage image, qreal start, qreal end)
{
    loadImage = image;
    rollStart = start;
    rollEnd = end;
    setParameters(loadImage.width(), loadImage.height());

    QImage dataImage;
    dataImage = QGLWidget::convertToGLFormat(loadImage);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dataImage.width(), dataImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, dataImage.bits());

    updateGL();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void Widget3D::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void Widget3D::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void Widget3D::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void Widget3D::initializeGL()
{
    initializeOpenGLFunctions();

    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    QColor clearColor = Qt::black;
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClearDepth(1.0f);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
}

void Widget3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(x, y, z);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.0f, 0.0f, -((rollEnd-rollStart)*7.5f) / 10000.0);
    gluCylinder(quadratic, ((float)loadImage.width())/(M_PI*600), ((float)loadImage.width())/(M_PI*600), loadImage.height()/300, 100, 100);
}



void Widget3D::resizeGL(int width, int height)
{
    if ( height == 0 )
    {
        height = 1;
    }

    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Widget3D::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Widget3D::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if (altDown)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            setXRotation(xRot + 8 * dx);
        }
        else if (event->buttons() & Qt::RightButton)
        {
            setZRotation(zRot + 8 * dx);
        }

    }
    else
    {
        if (event->buttons() & Qt::LeftButton)
        {
            if (qAbs(dx) > qAbs(dy))
            {
                setZRotation(zRot + 8 * dx);
            }
            else
            {
                setXRotation(xRot + 8 * dy);
            }

        } else if (event->buttons() & Qt::RightButton)
        {
            y -= dy / (100.0 / qAbs(z / 10));
            updateGL();
        }
    }
    lastPos = event->pos();
}

void Widget3D::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}

void Widget3D::wheelEvent(QWheelEvent *event)
{
    float numDegrees = (float)event->delta();
    z += numDegrees/100;
    updateGL();
}


void Widget3D::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_L:
        {
            light = !light;
            if (!light)
            {
                glDisable(GL_LIGHTING);
            }
            else
            {
                glEnable(GL_LIGHTING);
            }
            updateGL();
            break;
        }
        case Qt::Key_F2:
        {
            fullScreen = !fullScreen;
            if ( fullScreen )
            {
            showFullScreen();
            }
            else
            {
            showNormal();
            setGeometry((QApplication::desktop()->width() - width())/2,
                (QApplication::desktop()->height() - height())/2, 640, 480);
            }
            updateGL();
            break;
        }
        case Qt::Key_PageDown:
        {
            z -= 0.2f;
            updateGL();
            break;
        }
        case Qt::Key_PageUp:
        {
            z += 0.2f;
            updateGL();
            break;
        }
        case Qt::Key_Up:
        {
            xSpeed += 0.5;
            setXRotation(xRot + 8 * xSpeed);
            updateGL();
            break;
        }
        case Qt::Key_Alt:
        {
            altDown = 1;
            break;
        }
        case Qt::Key_Down:
        {
            xSpeed += 0.5;
            setXRotation(xRot - 8 * xSpeed);
            updateGL();
            break;
        }
        case Qt::Key_Right:
        {
            x += 0.5f;
            updateGL();
            break;
        }
        case Qt::Key_Left:
        {
            x -= 0.5f;
            updateGL();
            break;
        }
        case Qt::Key_A:
        {
            zRot += 16.0f;
            updateGL();
            break;
        }
        case Qt::Key_D:
        {
            zRot -= 16.0f;
            updateGL();
            break;
        }
        case Qt::Key_Escape:
        {
            showNormal();
            setGeometry((QApplication::desktop()->width() - width())/2,
              (QApplication::desktop()->height() - height())/2, 640, 480 );
        }
        default:
            break;
    }
}

void Widget3D::keyReleaseEvent(QKeyEvent *event)
{
    if (Qt::Key_Alt == event->key())
    {
        altDown = false;
    }
    else if ((Qt::Key_Up == event->key()) || (Qt::Key_Up == event->key()))
    {
        xSpeed = 1.0;
    }
}


void Widget3D::setParameters(int width, int height)
{
    xRot = -1100.0;
    yRot = 0;
    zRot = 0;
    x = 0;
    z = -height / 180;
    y = 0;
}

void Widget3D::handleZoomIn()
{
    z += 1.2;
    updateGL();
}

void Widget3D::handleZoomOut()
{
    z -= 1.2;
    updateGL();
}

void Widget3D::handleLeftSpin()
{
    setZRotation(zRot - 50);
}

void Widget3D::handleRightSpin()
{
    setZRotation(zRot + 50);
}

void Widget3D::startLeftSpin()
{
    timerRight.stop();
    timerLeft.start();
}

void Widget3D::startRightSpin()
{
    timerLeft.stop();
    timerRight.start();
}

void Widget3D::stopAutoSpin()
{
    timerLeft.stop();
    timerRight.stop();
}
