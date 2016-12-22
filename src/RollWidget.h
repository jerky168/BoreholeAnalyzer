#pragma  once
#include <QGLWidget>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QCloseEvent>

#include <GL/glu.h>		// Header File For The Glaux Library

class RollWidget : public QGLWidget
{
    Q_OBJECT
public:
    RollWidget(QWidget *parent = Q_NULLPTR, bool fs = false);
    ~RollWidget();


protected:
	bool imageIsNull();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void setXRotation(int angle);
	void setYRotation(int angle);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void DrawCube();
    void timerEvent(QTimerEvent *e);
    void setZRotation(int angle);

	void wheelEvent(QWheelEvent*);
	void closeEvent ( QCloseEvent * event );
signals:
	void signalTextureLoadFailed();

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);


    void setImage(QImage image, qreal start, qreal end);

private:
	bool altDown;
    QImage loadImage;
	bool fullscreen;
	bool light;

    qreal rollStart;
    qreal rollEnd;

	QPoint lastPos;
	GLfloat xrot;				// X Rotation
	GLfloat yrot;				// Y Rotation
	GLfloat zrot;
	GLfloat xspeed;				// X Rotation Speed
	GLfloat yspeed;				// Y Rotation Speed
	GLfloat z;			// Depth Into The Screen
	GLfloat x;
	GLfloat y;

    GLUquadricObj *quadratic;	 // 二次几何体

	void setParameters(int width, int heigth);

	inline int next_p2(int a)
	{
		int rval=1;
		while((rval << 1)<a) rval<<=1;
		return rval;
    }
};
