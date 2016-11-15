#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


#include <QUndoCommand>
#include <QUndoStack>


// database handler
#include "dbhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

typedef enum{
    IDLE,
    Opened
}AppStatus;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void switchImage(quint16 index);


private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

private:
    Ui::MainWindow *ui;

    DbHandler *handler;
    QUndoStack *undosStack;


    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;


};

#endif // MAINWINDOW_H
