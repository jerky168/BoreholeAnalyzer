#pragma once

#include <QGraphicsView>
class QMouseEvent;
class QWheelEvent;
class GraphicsView : public QGraphicsView
{
	Q_OBJECT
		
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
	
protected:
	void wheelEvent(QWheelEvent *event);


private:
	double factor;
	double totalFactor;
};
