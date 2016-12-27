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
//    ToolBar *toolbar = new ToolBar();
//    addToolBar(Qt::TopToolBarArea, toolbar);
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
    QObject::connect(scene, SIGNAL(showStatus(QString)), this, SLOT(showStatus(QString)));
    QObject::connect(scene, SIGNAL(showRealInfo(QString)), ui->defectWidget, SLOT(showRealInfo(QString)));
    QObject::connect(scene, SIGNAL(emitTableData(QVector<GraphicsScene::TableData>)), ui->defectWidget, SLOT(updateTableData(QVector<GraphicsScene::TableData>)));
    QObject::connect(scene, SIGNAL(update3DImage(QImage,qreal,qreal)), ui->widget3D, SLOT(setImage(QImage,qreal,qreal)));

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
    if (scene->hasNewItem())
    {
        QMap<QString, QGraphicsItem *> items = scene->getNewItems();
        QStringList keys = items.keys();
        for (int i = 0; i < keys.count(); i++)
        {
            QUuid uuid = QUuid(keys.at(i));
            quint16 index = ImageWidget::index;
            QGraphicsItem *item = items.value(uuid.toString());
            quint8 type = item->type();
            QString dataStr;
            switch (item->type())
            {
                case Angle:
                {
                    GraphicsAngleItem *i = dynamic_cast<GraphicsAngleItem *>(item);
                    dataStr = i->getDataString();
                    break;
                }

                case AnyShape:
                {
                    GraphicsAnyshape *i = dynamic_cast<GraphicsAnyshape *>(item);
                    dataStr = i->getDataString();
                    break;
                }
                case Ruler:
                {
                    GraphicsLineItem *i = dynamic_cast<GraphicsLineItem *>(item);
                    dataStr = i->getDataString();
                    break;
                }
                case Occurance:
                {
                    GraphicsOccurance *i = dynamic_cast<GraphicsOccurance *>(item);
                    dataStr = i->getDataString();
                    break;
                }
                case Rect:
                {
                    GraphicsRectItem *i = dynamic_cast<GraphicsRectItem *>(item);
                    dataStr = i->getDataString();
                    break;
                }

                case Text:
                {
                    GraphicsTextItem *i = dynamic_cast<GraphicsTextItem *>(item);
                    dataStr = i->getDataString();
                    break;
                }

                default:
                {
                    break;
                }
            }
            handler->saveItem(uuid, index, type, dataStr);
        }
        scene->saveNewItems();
    }
}


void MainWindow::on_actionExportImage_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "get save file", QDir::homePath(), tr("Images (*.jpg)"));
    DbHandler::IndexData indexData = handler->getIndexData(0);
    QImage image = GraphicsScene::getImageFromData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
    image.save(filename, "JPG");
}



void MainWindow::on_actionExportWord_triggered()
{
    QWord word;

    if (!word.createNewWord())
    {
        QMessageBox::critical(this, tr("Export report failed"),
                              tr("Export report failed, please confirm if the computer has installed Microsoft Office Word!"));
        return;
    }

    word.setPageOrientation(0);
    word.setWordPageView(3);
    word.setMargin(72, 72, 54, 54);

    //Header
    word.intsertTable(1, 2);

    word.setCellString(1, 1, tr("报告名称"));
    word.setCellString(1, 2, tr("Test"));

    word.moveForEnd();

    word.intsertTable(2, 6);
    word.setCellString(2, 1, tr("测孔编号"));
    word.setCellString(2, 3, tr("测试地点"));
    word.setCellString(2, 5, tr("测试时间"));

    word.setCellString(3, 1, tr("测孔直径"));
    word.setCellString(3, 3, tr("测试深度"));
    word.setCellString(3, 5, tr("始测深度"));

    word.moveForEnd();

    word.intsertTable(1, 8);
    word.setCellString(4, 1, tr("位置"));
    word.setCellString(4, 2, tr("深度"));
    word.setCellString(4, 3, tr("展开图"));
    word.setCellString(4, 4, tr("备注"));
    word.setCellString(4, 5, tr("位置"));
    word.setCellString(4, 6, tr("深度"));
    word.setCellString(4, 7, tr("展开图"));
    word.setCellString(4, 8, tr("备注"));
    word.moveForEnd();

    QImage image;
    for (int i = 0; i <= ImageWidget::maxIndex; i++)
    {
        image = getSceneImage(i);
        QString filename = QDir::temp().filePath("temp.jpg");
        image.save(filename, "JPG");
        word.insertPic(filename);
        word.insertMoveDown();
    }

    word.save();
}



void MainWindow::on_actionExportExcel_triggered()
{
     QExcel my_excel;
//    if (!my_excel.Open("/", 1, true))
//    {
//        QMessageBox::critical(this, tr("Export table failed"),
//                              tr("Export table failed, please confirm if the computer has installed Microsoft Office Excel!"));
//        return;
//    }

//    my_excel.Save();

//    QString xlsFile = "/";                            //默认路径
//    bool isExcelOpen = my_excel.Open(xlsFile, true);

//    QVector<DefectWidget::ItemData>     currItemsData;//当前页面数据

//    quint16                             currIndex = 0;//当前页面位置
//    quint16                               currdataNum;//当前页第几个数据
//    quint16                          aggregateNum = 1;//数据总数
//    quint16                                   dataNum;//当前数据个数
//    quint16                                    rowNum;//当前写入数据行数
//    QString                                  intToStr;//转换int为QString
//    if ( isExcelOpen )
//    {
//        //初始化表头和显示数据
//        my_excel.setCellString(1,1,"当前所有数据的Excel报表");
//        my_excel.mergeCells(1,1,4,4);

//        my_excel.setCellString(5, 1, "页数");
//        my_excel.setCellString(5, 2, "类型");
//        my_excel.setCellString(5, 3, "数据");
//        my_excel.setCellString(5, 4, "uuid");

//        while( currIndex < ImageWidget::maxIndex )
//        {
//            currItemsData = handler->getIndexData(currIndex).items;
//            for (dataNum = 0, currdataNum = 0; dataNum < currItemsData.count(); dataNum++,currdataNum++ )
//            {
//                rowNum = 5 + aggregateNum + currdataNum;

//                my_excel.setCellString(rowNum, 1, intToStr.setNum(currIndex + 1));
//                my_excel.setCellString(rowNum, 2, intToStr.setNum(currItemsData.at(dataNum).item->type()));
////                my_excel.setCellString(rowNum, 3, my_excel.GetExcelData(currItemsData.at(dataNum).item));
//                my_excel.setCellString(rowNum, 4, currItemsData.at(dataNum).uuid.toString());
//            }
//            aggregateNum += currdataNum;
//            currIndex++;
//        }
////        my_excel.setRowColumnAuto();
//        my_excel.Close();
//    }
//    else
//    {
//        qDebug("open failed\n");
//    }

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
    if (scene->hasNewItem())
    {
        QMessageBox messageBox(QMessageBox::Warning, tr("Unsave changes"),
                               tr("You have unsaved changes, switching index will discard theses changes!"),
                               QMessageBox::Discard | QMessageBox::Cancel, this);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        messageBox.setButtonText (QMessageBox::Discard, tr("Discard"));
        messageBox.setButtonText (QMessageBox::Cancel, tr("Cancel"));
        if (QMessageBox::Cancel == messageBox.exec())
        {
            ui->imageWidget->cancelSwitch();
            return;
        }

    }

    DbHandler::IndexData indexData = handler->getIndexData(index);
    scene->updateIndexData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
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

void MainWindow::showStatus(QString message)
{
    ui->statusBar->showMessage(message, 0);
}



QImage MainWindow::getSceneImage(quint16 index)
{
    DbHandler::IndexData indexData = handler->getIndexData(index);
    return GraphicsScene::getImageFromData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
}

QMap<QString, QGraphicsItem *> MainWindow::index2Item(DbHandler::IndexData indexData)
{
    QMap<QString, QGraphicsItem *> items;
    for (int i = 0; i < indexData.itemDatas.count(); i++)
    {
        QUuid uuid = indexData.itemDatas.at(i).uuid;
        qint32 type = indexData.itemDatas.at(i).type;
        QString dataStr = indexData.itemDatas.at(i).dataStr;
        switch (type)
        {
            case Angle:
            {
                GraphicsAngleItem *item = GraphicsAngleItem::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            case AnyShape:
            {
                GraphicsAnyshape *item = GraphicsAnyshape::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            case Ruler:
            {
                GraphicsLineItem *item = GraphicsLineItem::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            case Occurance:
            {
                GraphicsOccurance *item = GraphicsOccurance::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            case Rect:
            {
                GraphicsRectItem *item = GraphicsRectItem::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            case Text:
            {
                GraphicsTextItem *item = GraphicsTextItem::loadFromString(dataStr);
                item->setFinished();
                items.insert(uuid.toString(), item);
                break;
            }
            default:
                break;
        }
    }
    return items;
}

