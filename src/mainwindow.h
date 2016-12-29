#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>


#include "GraphicsSettings.h"
#include "GraphicsScene.h"
#include "DbHandler.h"
#include "QWord.h"
#include "QExcel.h"
#include "PrjInfoDialog.h"
#include "AboutDialog.h"
#include "RollWidget.h"

#include "app.h"


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
     void showStatus(QString message);

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_triggered();

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

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();


private:
    Ui::MainWindow *ui;
    DbHandler *handler;
    GraphicsScene *scene;
    PrjInfoDialog *infoDialog;

    QActionGroup *actionGroupMode;  //2D, 3D模式切换
    QActionGroup *actionGroup2D;    //2D模式下的操作
    QActionGroup *actionGroup3D;    //3D模式下的操作


    void createActionGroups();
    void createSceneAndView();
    void createConnections();
    void resetActions();


    QImage getSceneImage(quint16 index);
    QImage getPixmapImage(quint16 index);
    QMap<QString, QGraphicsItem *> index2Item(DbHandler::IndexData indexData);


signals:
    void clearScene();

    void updatePrjInfo(DbHandler::PrjInfo prjjInfo);
    void clearPrjInfo();

    void sigZoomIn();
    void sigZoomOut();

};

