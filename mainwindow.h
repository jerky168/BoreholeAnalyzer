#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QUndoCommand>
#include <QUndoStack>
#include <QActionGroup>
#include <QMessageBox>

#include "GraphicsScene.h"
#include "DbHandler.h"
#include "qword.h"
#include "qexcel.h"
#include "PrjInfoDialog.h"

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
     void handleModeChanged(GraphicsScene::Mode curMode);

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_triggered();

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void on_actionShift_triggered();
    void on_actionSlitWidth_triggered();
    void on_actionRectangle_triggered();
    void on_actionAnyShape_triggered();
    void on_actionOccurrence_triggered();
    void on_actionTextbox_triggered();
    void on_actionCross_triggered();

    void on_actionExportForm_triggered();
    void on_actionExportExcel_triggered();
    void on_actionProjectInfo_triggered();

private:
    Ui::MainWindow *ui;
    DbHandler *handler;
    QUndoStack *undosStack;
    GraphicsScene *scene;

    PrjInfoDialog *infoDialog;

    QWord   my_word;
    QExcel my_excel;
    QActionGroup *actionGroup;
    QActionGroup *editActionGroup;

    typedef enum{
        IDLE,
        Opened
    }AppStatus;

    void createActionGroups();
    void createSceneAndView();
    void createConnections();
    void resetActions();


signals:
    void clearScene();



};

#endif // MAINWINDOW_H
