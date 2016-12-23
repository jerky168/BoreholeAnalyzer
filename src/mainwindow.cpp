#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "PrjInfoDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handler(new DbHandler(this)),
    scene(new GraphicsScene(this)),
    actionGroupMode(new QActionGroup(this)),
    actionGroup2D(new QActionGroup(this)),
    actionGroup3D(new QActionGroup(this))
{
    ui->setupUi(this);

    createUI();
    createActionGroups();
    createSceneAndView();
    createConnections();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete handler;
    delete scene;
}

void MainWindow::createUI()
{
    ToolBar *toolbar = new ToolBar();
    addToolBar(Qt::TopToolBarArea, toolbar);
}


// create action group
void MainWindow::createActionGroups()
{
    //add the toobar and dock windows to menu view
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetImage->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetDefect->toggleViewAction());

    // make the 2D view and 3D view exclusive
    actionGroupMode->addAction(ui->action2DView);
    actionGroupMode->addAction(ui->action3DView);

    // make edit action exlusive
    actionGroup2D->addAction(ui->actionShift);
    actionGroup2D->addAction(ui->actionSlitWidth);
    actionGroup2D->addAction(ui->actionRectangle);
    actionGroup2D->addAction(ui->actionAnyShape);
    actionGroup2D->addAction(ui->actionOccurrence);
    actionGroup2D->addAction(ui->actionTextbox);
    actionGroup2D->addAction(ui->actionCross);

    // add all 3D action together
    actionGroup3D->setExclusive(false);
    actionGroup3D->addAction(ui->actionLeftSpin);
    actionGroup3D->addAction(ui->actionRightSpin);
    actionGroup3D->addAction(ui->actionAutoLeftSpin);
    actionGroup3D->addAction(ui->actionAutoRightSpin);

    actionGroup2D->setEnabled(true);
    actionGroup3D->setEnabled(false);
}


// graphics view
void MainWindow::createSceneAndView()
{
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(QBrush(Qt::white));
}

// create connections
void MainWindow::createConnections()
{
    //switch 2D view and 3D view
    connect(ui->action2DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->action2DView, &QAction::triggered, actionGroup2D, &QActionGroup::setEnabled);
    connect(ui->action2DView, &QAction::triggered, actionGroup3D, &QActionGroup::setDisabled);

    connect(ui->action3DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->action3DView, &QAction::triggered, actionGroup2D, &QActionGroup::setDisabled);
    connect(ui->action3DView, &QAction::triggered, actionGroup3D, &QActionGroup::setEnabled);


    // 切换照片
    QObject::connect(ui->imageWidget, SIGNAL(sigSwitchImage(quint16)), this, SLOT(switchImage(quint16)));

    QObject::connect(this, SIGNAL(clearScene()), scene, SLOT(clearScene()));
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), this, SLOT(handleModeChanged(GraphicsScene::Mode)));
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), ui->graphicsView, SLOT(handleModeChanged(GraphicsScene::Mode)));
    QObject::connect(scene, SIGNAL(showStatus(QString, int)), ui->statusBar, SLOT(showMessage(QString, int)));
    QObject::connect(scene, SIGNAL(showRealInfo(QString)), ui->defectWidget, SLOT(showRealInfo(QString)));
    QObject::connect(scene, SIGNAL(itemInserted(QGraphicsItem *, QUuid)), ui->defectWidget, SLOT(itemInserted(QGraphicsItem *, QUuid)));


    QObject::connect(this, SIGNAL(update3DImage(QImage,qreal,qreal)), ui->widget3D, SLOT(setImage(QImage,qreal,qreal)));
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open project file", QDir::homePath(), "Project file (*.ylink)");
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        on_actionClose_triggered();
    }

    if (!handler->openDatabase(filename))
        return;

    ui->actionSave->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    ui->imageWidget->updatePrjInfo(prjInfo);
    ui->actionClose->setEnabled(true);
}

void MainWindow::on_actionClose_triggered()
{
    emit clearScene();
    ui->imageWidget->clear();
    ui->actionClose->setEnabled(false);

    if (handler->isOpened())
        handler->closeDatabase();
}


void MainWindow::on_actionSave_triggered()
{
    if (ui->defectWidget->hasAddedItem())
    {
        QVector<DefectWidget::ItemData> items = ui->defectWidget->getAddedItems();
        for (int i = 0; i < items.count(); i++)
        {
            handler->saveItem(ImageWidget::index, items.at(i).uuid, items.at(i).item);
        }
        ui->defectWidget->clearAddedItems();
    }
}


void MainWindow::on_actionExportImage_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "get save file", QDir::homePath(), tr("Images (*.jpg)"));
    DbHandler::IndexData data = handler->getIndexData(0);
    QImage image = GraphicsScene::getImageFromData(data.image.pixmap, data.image.start, data.image.end, data.items);
    image.save(filename, "JPG");
}



void MainWindow::on_actionExportWord_triggered()
{
    if (!my_word.createNewWord())
    {
        QMessageBox::warning(this, "erorr", "error");
        return;
    }

    QImage image;
    for (int i = 0; i <= ImageWidget::maxIndex; i++)
    {
        image = handler->getSceneImage(i);
        QString filename = QDir::temp().filePath("temp.jpg");
        image.save(filename, "JPG");
        my_word.insertPic(filename);
        my_word.insertMoveDown();
    }

    my_word.save();
}



void MainWindow::on_actionExportExcel_triggered()
{
    QString xlsFile = "/";                            //默认路径
    bool isExcelOpen = my_excel.Open(xlsFile, true);

    QVector<DefectWidget::ItemData>     currItemsData;//当前页面数据

    quint16                             currIndex = 0;//当前页面位置
    quint16                               currdataNum;//当前页第几个数据
    quint16                          aggregateNum = 1;//数据总数
    quint16                                   dataNum;//当前数据个数
    quint16                                    rowNum;//当前写入数据行数
    QString                                  intToStr;//转换int为QString
    if ( isExcelOpen )
    {
        //初始化表头和显示数据
        my_excel.setCellString(1,1,"当前所有数据的Excel报表");
        my_excel.mergeCells(1,1,4,4);

        my_excel.setCellString(5, 1, "页数");
        my_excel.setCellString(5, 2, "类型");
        my_excel.setCellString(5, 3, "数据");
        my_excel.setCellString(5, 4, "uuid");

        while( currIndex < ImageWidget::maxIndex )
        {
            currItemsData = handler->getIndexData(currIndex).items;
            for (dataNum = 0, currdataNum = 0; dataNum < currItemsData.count(); dataNum++,currdataNum++ )
            {
                rowNum = 5 + aggregateNum + currdataNum;

                my_excel.setCellString(rowNum, 1, intToStr.setNum(currIndex + 1));
                my_excel.setCellString(rowNum, 2, intToStr.setNum(currItemsData.at(dataNum).item->type()));
//                my_excel.setCellString(rowNum, 3, my_excel.GetExcelData(currItemsData.at(dataNum).item));
                my_excel.setCellString(rowNum, 4, currItemsData.at(dataNum).uuid.toString());
            }
            aggregateNum += currdataNum;
            currIndex++;
        }
//        my_excel.setRowColumnAuto();
        my_excel.Close();
    }
    else
    {
        qDebug("open failed\n");
    }

}

void MainWindow::on_actionProjectInfo_triggered()
{
    PrjInfoDialog infoDialog;

    //TODO: initial the project info dialog
//    infoDialog.updatePrjInfo(prjInfo);

    infoDialog.exec();
}


void MainWindow::switchImage(quint16 index)
{
    if (ui->defectWidget->hasAddedItem())
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Unsave items"), tr("You have unsaved items, switching index will discard theses changes!"), QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (button == QMessageBox::Cancel)
        {
            ui->imageWidget->cancelSwitch();
            return;
        }

    }

    DbHandler::IndexData indexData = handler->getIndexData(index);
    ui->defectWidget->updateItems(indexData.items);
    scene->updateIndexData(indexData.image.pixmap, indexData.image.start, indexData.image.end, indexData.items);
    emit update3DImage(scene->getSceneImageFor3D(), indexData.image.start, indexData.image.end);
}



void MainWindow::on_actionUndo_triggered()
{

}

void MainWindow::on_actionRedo_triggered()
{

}


void MainWindow::on_actionShift_triggered()
{
    scene->setCurMode(GraphicsScene::InsertShift);
}

void MainWindow::on_actionSlitWidth_triggered()
{
    scene->setCurMode(GraphicsScene::InsertLine);
}

void MainWindow::on_actionRectangle_triggered()
{
    scene->setCurMode(GraphicsScene::InsertRectangle);
}

void MainWindow::on_actionAnyShape_triggered()
{
    scene->setCurMode(GraphicsScene::InsertAnyShape);
}

void MainWindow::on_actionOccurrence_triggered()
{
    scene->setCurMode(GraphicsScene::InsertOccurance);
}

void MainWindow::on_actionTextbox_triggered()
{
    scene->setCurMode(GraphicsScene::InsertTextBox);
}

void MainWindow::on_actionCross_triggered()
{
    scene->setCurMode(GraphicsScene::InsertCross);
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionManual_triggered()
{
}

void MainWindow::on_actionContact_triggered()
{

}

void MainWindow::resetActions()
{
    for (quint8 i = 0; i < actionGroup2D->actions().count(); i++)
        actionGroup2D->actions()[i]->setChecked(false);
}


void MainWindow::handleModeChanged(GraphicsScene::Mode curMode)
{
    if (curMode == GraphicsScene::MoveItem)
    {
        resetActions();
    }
}


