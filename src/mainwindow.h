#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QUndoCommand>
#include <QUndoStack>
#include <QActionGroup>
#include <QMessageBox>

#include "ToolBar.h"

#include "GraphicsScene.h"
#include "DbHandler.h"
#include "QWord.h"
#include "QExcel.h"
#include "PrjInfoDialog.h"
#include "AboutDialog.h"

#include "RollWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
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

    void on_actionExportImage_triggered();
    void on_actionExportWord_triggered();
    void on_actionExportExcel_triggered();
    void on_actionProjectInfo_triggered();

    void on_actionAbout_triggered();

    void on_actionManual_triggered();

    void on_actionContact_triggered();

private:
    Ui::MainWindow *ui;
    DbHandler *handler;
    GraphicsScene *scene;

    ToolBar *toolbar;
    PrjInfoDialog *infoDialog;

    QActionGroup *actionGroup;
    QActionGroup *editActionGroup;

    QWord my_word;
    QExcel my_excel;



    void createUI();
    void createActionGroups();
    void createSceneAndView();
    void createConnections();
    void resetActions();

signals:
    void clearScene();

    void update3DImage(QImage image, qreal start, qreal end);


};

