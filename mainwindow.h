#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QUndoCommand>
#include <QUndoStack>
#include <QActionGroup>

#include "GraphicsScene.h"

#include "DbHandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void switchImage(quint16 index);
     void modeChanged(GraphicsScene::Mode lastMode, GraphicsScene::Mode curMode);

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void on_actionSlitWidth_triggered();
    void on_actionRectangle_triggered();
    void on_actionAnyShape_triggered();
    void on_actionOccurrence_triggered();
    void on_actionTextbox_triggered();
    void on_actionCross_triggered();

private:
    Ui::MainWindow *ui;

    DbHandler *handler;
    QUndoStack *undosStack;
    GraphicsScene *scene;

    QActionGroup *actionGroup;
    QActionGroup *editActionGroup;

    typedef enum{
        IDLE,
        Opened
    }AppStatus;


    void createConnections();
    void resetActions();


signals:
    void updatePixmap(QPixmap pixmap);
    void clearPixmap();



};

#endif // MAINWINDOW_H
