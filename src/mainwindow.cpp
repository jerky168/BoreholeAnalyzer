#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "PrjInfoDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handler(new DbHandler(this)),
    scene(new GraphicsScene(this)),
    actionGroup(new QActionGroup(this)),
    editActionGroup(new QActionGroup(this))
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
    actionGroup->addAction(ui->action2DView);
    actionGroup->addAction(ui->action3DView);

    // make edit action exlusive
    editActionGroup->addAction(ui->actionShift);
    editActionGroup->addAction(ui->actionSlitWidth);
    editActionGroup->addAction(ui->actionRectangle);
    editActionGroup->addAction(ui->actionAnyShape);
    editActionGroup->addAction(ui->actionOccurrence);
    editActionGroup->addAction(ui->actionTextbox);
    editActionGroup->addAction(ui->actionCross);
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
    QObject::connect(ui->action2DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(0);});
    QObject::connect(ui->action3DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(1);});

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
        QMessageBox::critical(this,
                              tr("Export report failed"),
                              tr("Export report failed, please confirm if the computer has installed Microsoft Word!")
                              );
        return;
    }

    word.setPageOrientation(0);
    word.setWordPageView(3);
    word.setMargin(72, 72, 54, 54);

    //Header
    word.insertTable(1, 2);

    word.setCellString(1, 1, tr("Name"));
    word.setCellString(1, 2, tr("Test"));

    word.moveForEnd();

    word.insertTable(2, 6);
    word.setCellString(2, 1, tr("Number"));
    word.setCellString(2, 3, tr("Site"));
    word.setCellString(2, 5, tr("Time"));

    word.setCellString(3, 1, tr("Diameter"));
    word.setCellString(3, 3, tr("Depth"));
    word.setCellString(3, 5, tr("StartDepth"));

    word.moveForEnd();

    word.insertTable(1, 6);
    word.setCellString(4, 1, tr("Position"));
    word.setCellString(4, 2, tr("Image"));
    word.setCellString(4, 3, tr("Remarks"));
    word.setCellString(4, 6, tr("Position"));
    word.setCellString(4, 5, tr("Image"));
    word.setCellString(4, 6, tr("Remarks"));
    word.moveForEnd();

    int rows = (ImageWidget::maxIndex+2)/2;

    for (int i = 0; i < rows; i++)
    {
        word.insertTable(1, 6);

        QImage image = getSceneImage(2*i);
        image = image.scaled(image.dotsPerMeterX() * 0.048, image.dotsPerMeterY() * 0.2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        image.save(QDir::temp().filePath("temp.jpg"));
        word.insertCellPic(i + 5, 2, QDir::temp().filePath("temp.jpg"));

        image = getSceneImage(2*i+1);
        image = image.scaled(image.dotsPerMeterX() * 0.048, image.dotsPerMeterY() * 0.2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        image.save(QDir::temp().filePath("temp.jpg"));
        word.insertCellPic(i + 5, 5, QDir::temp().filePath("temp.jpg"));

        word.moveForEnd();
    }


//    {
//        QAxObject* selection = word.querySubObject("Selection");
//        QVariantList params;
//        params.append(6);
//        params.append(0);
//        selection->dynamicCall("EndOf(QVariant&, QVariant&)", params).toInt();
//    }

//    {
//        QAxObject* tables = document->querySubObject("Tables");
//        QAxObject* selection = word.querySubObject("Selection");
//        QAxObject* range = selection->querySubObject("Range");
//        QVariantList params;
//        params.append(range->asVariant());
//        params.append(1);
//        params.append(1);
//        params.append(1);
//        params.append(2);
//        tables->querySubObject("Add(QAxObject*, int, int, QVariant&, QVariant&)", params);
//    }

//    {
//        QImage image = getSceneImage(0);
//        image = image.scaled(image.dotsPerMeterX() * 0.05, image.dotsPerMeterY() * 0.225, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        image.save(QDir::temp().filePath("temp.jpg"));

//        QAxObject *selection = word.querySubObject("Selection");
//        QAxObject *table = selection->querySubObject("Tables(1)");
//        QAxObject *range = table->querySubObject("Cell(int, int)",6,2)->querySubObject("Range");
//        range->querySubObject("InlineShapes")->dynamicCall("AddPicture(const QString&)", QDir::temp().filePath("temp.jpg"));
//    }



//    QImage image;
//    for (int i = 0; i <= ImageWidget::maxIndex; i++)
//    {
//        image = getSceneImage(i);
//        QString filename = QDir::temp().filePath("temp.jpg");
//        image.save(filename, "JPG");

//        QAxObject *selection = word.querySubObject("Selection");
//        QAxObject *shapes = selection->querySubObject("InlineShapes");
//        shapes->dynamicCall("AddPicture(const QString&)", filename);
//        selection->dynamicCall("TypeParagraph(void)");
//    }


//    document->dynamicCall("Save()");

    word.save();
//    CoUninitialize();
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

    workBook->dynamicCall("Save()");

    CoUninitialize();
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
    ManualDialog *dialog = new ManualDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionContact_triggered()
{
    ContactDialog *dialog = new ContactDialog(this);
    dialog->exec();
    delete dialog;
}



void MainWindow::resetActions()
{
    for (quint8 i = 0; i < editActionGroup->actions().count(); i++)
        editActionGroup->actions()[i]->setChecked(false);
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

