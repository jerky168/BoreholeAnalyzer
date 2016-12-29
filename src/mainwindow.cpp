#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handler(new DbHandler(this)),
    scene(new GraphicsScene(this)),
    actionGroupMode(new QActionGroup(this)),
    actionGroup2D(new QActionGroup(this)),
    actionGroup3D(new QActionGroup(this)),
    infoDialog(new PrjInfoDialog(this))
{
    ui->setupUi(this);

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


    QObject::connect(ui->imageWidget, SIGNAL(sigSwitchImage(quint16)), this, SLOT(switchImage(quint16)));

    QObject::connect(this, SIGNAL(clearScene()), scene, SLOT(clearScene()));
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), this, SLOT(handleModeChanged(GraphicsScene::Mode)));
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), ui->graphicsView, SLOT(handleModeChanged(GraphicsScene::Mode)));
    QObject::connect(scene, SIGNAL(showRealInfo(QString)), ui->defectWidget, SLOT(showRealInfo(QString)));
    QObject::connect(scene, SIGNAL(emitTableData(QVector<GraphicsScene::TableData>)), ui->defectWidget, SLOT(updateTableData(QVector<GraphicsScene::TableData>)));
    QObject::connect(scene, SIGNAL(update3DImage(QImage,qreal,qreal)), ui->widget3D, SLOT(setImage(QImage,qreal,qreal)));

    QObject::connect(this, SIGNAL(updatePrjInfo(DbHandler::PrjInfo)), ui->imageWidget, SLOT(updatePrjInfo(DbHandler::PrjInfo)));
    QObject::connect(this, SIGNAL(updatePrjInfo(DbHandler::PrjInfo)), infoDialog, SLOT(updatePrjInfo(DbHandler::PrjInfo)));
    QObject::connect(this, SIGNAL(clearPrjInfo()), ui->imageWidget, SLOT(clearPrjInfo()));
    QObject::connect(this, SIGNAL(clearPrjInfo()), infoDialog, SLOT(clearPrjInfo()));

    connect(this, SIGNAL(sigZoomIn()), ui->graphicsView, SLOT(handleZoomIn()));
    connect(this, SIGNAL(sigZoomOut()), ui->graphicsView, SLOT(handleZoomOut()));
    connect(this, SIGNAL(sigZoomIn()), ui->widget3D, SLOT(handleZoomIn()));
    connect(this, SIGNAL(sigZoomOut()), ui->widget3D, SLOT(handleZoomOut()));

    connect(ui->actionCross, SIGNAL(triggered(bool)), ui->graphicsView, SLOT(handleCrossMouse(bool)));

    connect(infoDialog, SIGNAL(savePrjInfo(DbHandler::PrjInfo)), handler, SLOT(setPrjInfo(DbHandler::PrjInfo)));

    connect(ui->defectWidget, SIGNAL(deleteItem(int)), scene, SLOT(deleteItem(int)));
    connect(scene, SIGNAL(deleteSaveItem(QUuid)), handler, SLOT(deleteItem(QUuid)));

}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open project file"), QDir::homePath(), tr("Project file (*.ylink)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        on_actionClose_triggered();
    }


    setWindowTitle(filename + " - " + App_Name);


    if (!handler->openDatabase(filename))
        return;

    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    emit updatePrjInfo(prjInfo);

    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionExportImage->setEnabled(true);
    ui->actionExportWord->setEnabled(true);
    ui->actionExportExcel->setEnabled(true);

    ui->actionAnyShape->setEnabled(true);
    ui->actionCross->setEnabled(true);
    ui->actionRectangle->setEnabled(true);
    ui->actionTextbox->setEnabled(true);
    ui->actionSlitWidth->setEnabled(true);
    ui->actionShift->setEnabled(true);
    ui->actionOccurrence->setEnabled(true);

    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);

    ui->actionProjectInfo->setEnabled(true);

    QObject::connect(scene, SIGNAL(showStatus(QString)), this, SLOT(showStatus(QString)));

}

void MainWindow::on_actionClose_triggered()
{
    emit clearScene();
    emit clearPrjInfo();


    setWindowTitle(App_Name);

    if (handler->isOpened())
        handler->closeDatabase();


    ui->actionClose->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionExportImage->setEnabled(false);
    ui->actionExportWord->setEnabled(false);
    ui->actionExportExcel->setEnabled(false);

    ui->actionAnyShape->setEnabled(false);
    ui->actionCross->setEnabled(false);
    ui->actionRectangle->setEnabled(false);
    ui->actionTextbox->setEnabled(false);
    ui->actionSlitWidth->setEnabled(false);
    ui->actionShift->setEnabled(false);
    ui->actionOccurrence->setEnabled(false);

    ui->actionZoomIn->setEnabled(false);
    ui->actionZoomOut->setEnabled(false);

    ui->actionProjectInfo->setEnabled(false);

    QObject::disconnect(scene, SIGNAL(showStatus(QString)), this, SLOT(showStatus(QString)));
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
    QString filename = QFileDialog::getSaveFileName(this, tr("Export Image"), QDir::homePath(), tr("Images files (*.jpg);;All files (*.*)"));
    DbHandler::IndexData indexData = handler->getIndexData(ImageWidget::index);
    QImage image = GraphicsScene::getImageFromData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
    image.save(filename, "JPG");
}


QImage handleImage(QImage image)
{
    return image;
}



void MainWindow::on_actionExportWord_triggered()
{
    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    QWord word;

    if (!word.createNewWord())
    {
        QMessageBox::critical(this,
                              tr("Export report failed"),
                              tr("Export report failed, please confirm if the computer has installed Microsoft Word!")
                              );
        return;
    }

    word.setPageOrientation(0);
    word.setWordPageView(3);
    word.setMargin(72, 72, 54, 54);

    word.insertTable(1, 2);

    word.setCellString(1, 1, tr("Name"));
    word.setCellString(1, 2, prjInfo.projectName);
    word.moveForEnd();

    word.insertTable(2, 6);
    word.setCellString(2, 1, tr("Number"));
    word.setCellString(2, 2, prjInfo.orificeNumber);
    word.setCellString(2, 3, tr("Site"));
    word.setCellString(2, 4, prjInfo.projectSite);
    word.setCellString(2, 5, tr("Time"));
    word.setCellString(2, 6, prjInfo.projectTime);

    word.setCellString(3, 1, tr("Diameter"));
    word.setCellString(3, 2, QString::number(prjInfo.diameter, 'f', 3) + "m");
    word.setCellString(3, 3, tr("Depth"));
    word.setCellString(3, 4, QString::number(prjInfo.endHeight, 'f', 3) + "m");
    word.setCellString(3, 5, tr("StartDepth"));
    word.setCellString(3, 6, QString::number(prjInfo.startHeight, 'f', 3) + "m");

    word.moveForEnd();

    word.insertTable(1, 6);
    word.setCellString(4, 1, tr("Position"));
    word.setCellString(4, 2, tr("Image"));
    word.setCellString(4, 3, tr("Remarks"));
    word.setCellString(4, 4, tr("Position"));
    word.setCellString(4, 5, tr("Image"));
    word.setCellString(4, 6, tr("Remarks"));
    word.moveForEnd();


    int rows = (ImageWidget::maxIndex+2)/2;
    for (int i = 0; i < rows; i++)
    {
        word.insertTable(1, 6);

        QImage image = getPixmapImage(2*i);
        image.setDotsPerMeterX(image.width() / 0.05);
        image.setDotsPerMeterY(image.width() / 0.05);
        image.save(QDir::temp().filePath("temp.jpg"));
        word.insertCellPic(i + 5, 2, QDir::temp().filePath("temp.jpg"));

        if ((ImageWidget::maxIndex + 1) % 2 == 1 && (i == rows - 1))
        {

        }
        else
        {
            image = getPixmapImage(2*i+1);
            image.setDotsPerMeterX(image.width() / 0.05);
            image.setDotsPerMeterY(image.width() / 0.05);
            image.save(QDir::temp().filePath("temp.jpg"));
            word.insertCellPic(i + 5, 5, QDir::temp().filePath("temp.jpg"));
        }


        word.moveForEnd();
    }

    word.save();
}



void MainWindow::on_actionExportExcel_triggered()
{   
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    QAxObject excel;
    if (!excel.setControl("Excel.Application"))
    {
        QMessageBox::critical(this,
                              tr("Export table failed"),
                              tr("Export table failed, please confirm if the computer has installed Microsoft Excel!")
                              );
        return;
    }

    excel.setProperty("Visible", true);
    QAxObject *workBooks = excel.querySubObject("WorkBooks");

    workBooks->dynamicCall("Add (void)");
    QAxObject *workBook = excel.querySubObject("ActiveWorkBook");
    QAxObject *worksheet = workBook->querySubObject("WorkSheets(int)", 1);

    quint32 itemCount = 2;
    QAxObject *range = worksheet->querySubObject("Cells(int,int)", 1, 1);
    range->dynamicCall("SetValue(const QString&)", tr("index"));
    range->setProperty("HorizontalAlignment", -4108);
    range = worksheet->querySubObject("Cells(int,int)", 1, 2);
    range->dynamicCall("SetValue(const QString&)", tr("type"));
    range->setProperty("HorizontalAlignment", -4108);
    range = worksheet->querySubObject("Cells(int,int)", 1, 3);
    range->dynamicCall("SetValue(const QString&)", tr("data1"));
    range->setProperty("HorizontalAlignment", -4108);
    range = worksheet->querySubObject("Cells(int,int)", 1, 4);
    range->dynamicCall("SetValue(const QString&)", tr("data2"));
    range->setProperty("HorizontalAlignment", -4108);


    for (int i = 0; i <= ImageWidget::maxIndex; i++)
    {
        DbHandler::IndexData indexData = handler->getIndexData(i);
        QVector<GraphicsScene::TableData> tableDatas = GraphicsScene::getTableDataFromData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
        for (int j = 0; j < tableDatas.count(); j++)
        {
            range = worksheet->querySubObject("Cells(int,int)", itemCount, 1);
            range->dynamicCall("SetValue(const QString&)", QString::number(i+1));
            range->setProperty("HorizontalAlignment", -4108);
            range = worksheet->querySubObject("Cells(int,int)", itemCount, 2);
            range->dynamicCall("SetValue(const QString&)", tableDatas.at(j).type);
            range->setProperty("HorizontalAlignment", -4108);
            range = worksheet->querySubObject("Cells(int,int)", itemCount, 3);
            range->dynamicCall("SetValue(const QString&)", tableDatas.at(j).data.section('\n', 0, 0));
            range->setProperty("HorizontalAlignment", -4131);
            range = worksheet->querySubObject("Cells(int,int)", itemCount, 4);
            range->dynamicCall("SetValue(const QString&)", tableDatas.at(j).data.section('\n', 1).replace("\n", "  "));
            range->setProperty("HorizontalAlignment", -4131);
            itemCount++;
        }
    }


    range = worksheet->querySubObject("UsedRange");
    QAxObject *cells = range->querySubObject("Columns");
    cells->dynamicCall("AutoFit");

    workBook->dynamicCall("Close (Boolen)", true);
    excel.dynamicCall("Quit(void)");

    CoUninitialize();
}

void MainWindow::on_actionProjectInfo_triggered()
{
    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    infoDialog->updatePrjInfo(prjInfo);
    infoDialog->exec();
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
    QDesktopServices::openUrl(QUrl(Website_Url));
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


QImage MainWindow::getPixmapImage(quint16 index)
{
    DbHandler::IndexData indexData = handler->getIndexData(index);
    return GraphicsScene::getPixmapImageFromData(indexData.image.pixmap, indexData.image.start, indexData.image.end, index2Item(indexData));
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


void MainWindow::on_actionZoomIn_triggered()
{
    emit sigZoomIn();
}

void MainWindow::on_actionZoomOut_triggered()
{
    emit sigZoomOut();
}

