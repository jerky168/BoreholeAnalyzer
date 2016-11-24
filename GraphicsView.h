#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QPinchGesture>
#include <QtMath>
#include <QPainter>

#include <QMouseEvent>
#include "GraphicsScene.h"


class GraphicsView : public QGraphicsView
{
	Q_OBJECT
		
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
    ~GraphicsView();

public slots:
    void handleItemInserted(QGraphicsItem* const &insertedItem);
    void handleItemDeleted(QGraphicsItem* const &deletedItem);

protected:
	void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *mouseEvent);

private:
	double factor;
	double totalFactor;


signals:
    void mouseMoved(QMouseEvent *event);

};
