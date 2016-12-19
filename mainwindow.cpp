#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    handler(new DbHandler(this)),
    undosStack(new QUndoStack(this)),
    scene(new GraphicsScene(this)),
    infoDialog(new PrjInfoDialog(this)),
    actionGroup(new QActionGroup(this)),
    editActionGroup(new QActionGroup(this))
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
    delete infoDialog;
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

    // 更新照片
    QObject::connect(this, SIGNAL(clearScene()), scene, SLOT(clearScene()));

    // 编辑模式改变
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), this, SLOT(handleModeChanged(GraphicsScene::Mode)));
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode)), ui->graphicsView, SLOT(handleModeChanged(GraphicsScene::Mode)));

    // 状态栏
    QObject::connect(scene, SIGNAL(showStatus(QString, int)), ui->statusBar, SLOT(showMessage(QString, int)));

    // 实时信息
    QObject::connect(scene, SIGNAL(showRealInfo(QString)), ui->defectWidget, SLOT(showRealInfo(QString)));

    // 当item添加之后
    QObject::connect(scene, SIGNAL(itemInserted(QGraphicsItem*,QUuid)), ui->defectWidget, SLOT(itemInserted(QGraphicsItem*,QUuid)));

}

void MainWindow::on_actionOpen_triggered()
{
    on_actionClose_triggered();

    QString filename = QFileDialog::getOpenFileName(this, "打开工程文件", QDir::homePath(), "工程文件 (*.ylink)");
    if (!handler->openDatabase(filename))
        return;

    ui->actionSave->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    ui->imageWidget->updatePrjInfo(prjInfo);
    ui->actionClose->setEnabled(true);
    infoDialog->updatePrjInfo(prjInfo);
}

void MainWindow::on_actionClose_triggered()
{
    emit clearScene();
    ui->imageWidget->clear();
    ui->actionClose->setEnabled(false);
    infoDialog->clearPrjInfo();

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


QImage MainWindow::getSceneImage(quint16 index)
{
    DbHandler::IndexData data = handler->getIndexData(index);
    return GraphicsScene::getImageFromData(data.image.pixmap, data.image.start, data.image.end, data.items);
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
        image = getSceneImage(i);
        QString filename = QDir::temp().filePath("temp.jpg");
        image.save(filename, "JPG");
        my_word.insertPic(filename);
        my_word.insertMoveDown();
    }

    my_word.save();
}


void MainWindow::on_actionProjectInfo_triggered()
{
    infoDialog->exec();
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
    scene->updateIndexData(indexData.image.pixmap, (qreal)(indexData.image.start) / 10000, (qreal)(indexData.image.end) / 10000, indexData.items);
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




