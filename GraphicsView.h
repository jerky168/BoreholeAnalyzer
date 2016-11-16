#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>


class QMouseEvent;
class QWheelEvent;
class GraphicsView : public QGraphicsView
{
	Q_OBJECT
		
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
	
public slots:
    void updatePixmap(QPixmap pixmap);
    void clearPixmap();


protected:
	void wheelEvent(QWheelEvent *event);


private:
	double factor;
	double totalFactor;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;

};
