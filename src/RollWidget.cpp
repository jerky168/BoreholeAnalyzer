#include "RollWidget.h"
#include <QImage>
#include <QMessageBox>
#include <stdio.h>
#include <QDebug>
#include <QFileDialog>

//Ambient Light是模拟漫反射的一种光源。它能将灯光均匀地照射在场景中每个物体上面，
//在使用Ambient Light时可以忽略方向和角度，只考虑光源的位置。
GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };

//Diffuse Light 检索或设置光线发出的漫射色。
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };

//光源位置
GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };


#define pi 3.1415926


RollWidget::RollWidget(QWidget *parent, bool fs) :
    QGLWidget(parent),
    fullscreen(fs),
    x(2),
    y(0),
    z(0),
    xrot(0),
    yrot(0),
    zrot(0),
    altDown(0),
    xspeed(1.0),
    light(false),
    rollStart(0),
    rollEnd(0)
{
    setMinimumSize(600, 800);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_DeleteOnClose);

    loadImage = QImage(1024, 1024, QImage::Format_ARGB32);
    loadImage.fill(Qt::blue);

    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);
}

RollWidget::~RollWidget()
{

}

QSize RollWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize RollWidget::sizeHint() const
{
    return QSize(400, 400);
}

void RollWidget::timerEvent(QTimerEvent *)
{
    updateGL();
}

void RollWidget::initializeGL()
{
    QImage dataImage;
    dataImage = QGLWidget::convertToGLFormat(loadImage);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dataImage.width(), dataImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, dataImage.bits());


    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);

    return;
}


void RollWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(x, y, z);

    glRotatef(xrot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yrot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zrot / 16.0, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.0f, 0.0f, -((rollEnd-rollStart)*7.5f) / 10000.0);
    gluCylinder(quadratic, ((float)loadImage.width())/(pi*600), ((float)loadImage.width())/(pi*600), loadImage.height()/300, 32, 32);
}



void RollWidget::resizeGL( int width, int height )
{
    if ( height == 0 )
    {
        height = 1;
    }

    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void RollWidget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
    case Qt::Key_L:
		{
			light=!light;
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
			case Qt::Key_F2:
			fullscreen = !fullscreen;
			if ( fullscreen )
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
			xspeed += 0.5;
			setXRotation(xrot + 8 * xspeed);
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
			xspeed += 0.5;
			setXRotation(xrot - 8 * xspeed);
            updateGL();
            break;
        }
  case Qt::Key_Right:
        {
            //yspeed += 0.1f;
            x += 0.5f;
            updateGL();
            break;
        }
  case Qt::Key_Left:
        {
            //yspeed -= 0.1f;
            x -= 0.5f;
            updateGL();
            break;
        }
  case Qt::Key_A:
      {
          //yspeed += 0.1f;
          zrot += 16.0f;
          updateGL();
          break;
      }
  case Qt::Key_D:
      {
          //yspeed += 0.1f;
          zrot -= 16.0f;
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

void RollWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void RollWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xrot) 
    {
        xrot = angle;
        updateGL();
    }
}

void RollWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yrot) 
    {
        yrot = angle;
        updateGL();
    }
}

void RollWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zrot) 
    {
        zrot = angle;
        updateGL();
    }
}


void RollWidget::setImage(QImage image, qreal start, qreal end)
{
    rollStart = start;
    rollEnd = end;

    loadImage = image;
	
//    int nWidthPowerOfTwo = next_p2(loadImage.width());
//    int nHeightPowerOfTwo = next_p2(loadImage.height());

//    loadImage = loadImage.scaled(nWidthPowerOfTwo, nHeightPowerOfTwo, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    setParameters(loadImage.width(), loadImage.height());
	initializeGL();
	updateGL();
}

void RollWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
	if (true == altDown)
	{
		if (event->buttons() & Qt::LeftButton) 
		{
			//setXRotation(xrot + 8 * dy);
			setXRotation(xrot + 8 * dx);
		} else if (event->buttons() & Qt::RightButton)
		{
			setZRotation(zrot + 8 * dx);
		}

	}
	else
	{
		if (event->buttons() & Qt::LeftButton) 
		{
            if (qAbs(dx) > qAbs(dy))
            {
                setZRotation(zrot + 8 * dx);
            } 
            else
            {
                setXRotation(xrot + 8 * dy);
            }
			
		} else if (event->buttons() & Qt::RightButton)
		{
			//x += dx / 250.0;
			y -= dy / (100.0 / qAbs(z / 10));
			updateGL();
		}
	}
    lastPos = event->pos();
}

// void RollWidget::wheelRotatedEvent( QWheelEvent* event)
// {
//     float numDegrees = (float)event->delta();
//     z += numDegrees/600;
//     updateGL();
// }

bool RollWidget::imageIsNull()
{
    if (loadImage.isNull())
    {
        return true;
    }
    else
    {
        return false;
    }
}

// void RollWidget::mouseReleaseEvent( QMouseEvent * )
// {
// 	altDown = false;
// }

void RollWidget::keyReleaseEvent( QKeyEvent *event )
{
	if (Qt::Key_Alt == event->key())
	{
		altDown = false;
	}
	else if ((Qt::Key_Up == event->key()) || (Qt::Key_Up == event->key()))
	{
		xspeed = 1.0;
	} 
}

void RollWidget::closeEvent( QCloseEvent * event )
{
	event = 0;
}

void RollWidget::wheelEvent(QWheelEvent* event)
{
	float numDegrees = (float)event->delta();
	z += numDegrees/100;
	updateGL();
}

void RollWidget::setParameters(int width, int heigth)
{
	xrot = -1100.0;
	yrot = 0;
	zrot = 0;
	x = 0;
	z = -heigth / 180;
    y = 0;
	//y = -5 * ((-z) / 40);
}

