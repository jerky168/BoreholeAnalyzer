#pragma once

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include <QGraphicsSceneMouseEvent>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = Q_NULLPTR);
    ~GraphicsScene();

    enum Mode {Normal, Move, InsertText};

    Mode getCurMode() {return curMode;}
    void setCurMode(Mode mode) {curMode = mode;}

public slots:
    void updatePixmap(QPixmap pixmap);
    void clearPixmap();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QGraphicsPixmapItem *pixmapItem;
    Mode curMode;

};
