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
    void handleModeChanged(GraphicsScene::Mode curMode);

    void handleZoomIn();
    void handleZoomOut();
    void handleCrossMouse(bool checked);

protected:
	void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *mouseEvent);

private:
    // 放大倍数
	double factor;

    bool isCross;
    //double totalFactor;

signals:
    void mouseMoved(QMouseEvent *event);

};
