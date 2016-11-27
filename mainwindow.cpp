#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    undosStack(new QUndoStack(this)),
    scene(new GraphicsScene()),
    actionGroup(new QActionGroup(this)),
    editActionGroup(new QActionGroup(this))
{
    ui->setupUi(this);
    //add the toobar and dock windows to menu view
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetImage->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetDefect->toggleViewAction());

    // make the 2D view and 3D view exclusive
    actionGroup->addAction(ui->action2DView);
    actionGroup->addAction(ui->action3DView);

    // make edit action exlusive
    editActionGroup->addAction(ui->actionSlitWidth);
    editActionGroup->addAction(ui->actionRectangle);
    editActionGroup->addAction(ui->actionAnyShape);
    editActionGroup->addAction(ui->actionOccurrence);
    editActionGroup->addAction(ui->actionTextbox);
    editActionGroup->addAction(ui->actionCross);

    // graphics view
    ui->graphicsView->setScene(scene);

    // create connections
    createConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createConnections()
{
    //switch 2D view and 3D view
    QObject::connect(ui->action2DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(0);});
    QObject::connect(ui->action3DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(1);});

    // 切换照片
    QObject::connect(ui->imageWidget, SIGNAL(sigSwitchImage(quint16)), this, SLOT(switchImage(quint16)));

    // 更新照片
    QObject::connect(this, SIGNAL(updatePixmap(QPixmap)), scene, SLOT(updatePixmap(QPixmap)));
    QObject::connect(this, SIGNAL(clearPixmap()), scene, SLOT(clearPixmap()));

    // 编辑模式改变
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode, GraphicsScene::Mode)), this, SLOT(modeChanged(GraphicsScene::Mode, GraphicsScene::Mode)));

    // 当view中发生鼠标移动
    QObject::connect(ui->graphicsView, SIGNAL(mouseMoved(QMouseEvent *)), this, SLOT(handleMouseMoved(QMouseEvent *)));

    // scene的模式改变相关
    QObject::connect(this, SIGNAL(), scene, SLOT());
    QObject::connect(scene, SIGNAL(modeChanged(GraphicsScene::Mode, GraphicsScene::Mode)), ui->graphicsView, SLOT());

    // 当scene中添加了新的item时
    QObject::connect(scene, SIGNAL(itemInserted(QGraphicsItem* const &)), this, SLOT(handleItemInserted(QGraphicsItem* const &)));
    QObject::connect(scene, SIGNAL(itemInserted(QGraphicsItem* const &)), ui->graphicsView, SLOT(handleItemInserted(QGraphicsItem* const &)));
}

// 打开文件
void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开工程文件", QDir::homePath(), "工程文件 (*.ylink)");
    handler = new DbHandler(filename, this);
    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    ui->imageWidget->updatePrjInfo(prjInfo);
    ui->actionClose->setEnabled(true);
}

// 关闭文件
void MainWindow::on_actionClose_triggered()
{
    emit clearPixmap();
    ui->imageWidget->clear();
    ui->actionClose->setEnabled(false);
}


void MainWindow::switchImage(quint16 index)
{
    DbHandler::BigImage bigImage = handler->getBigImage(index);
    emit updatePixmap(bigImage.pixmap);
}



void MainWindow::on_actionUndo_triggered()
{

}

void MainWindow::on_actionRedo_triggered()
{

}


void MainWindow::on_actionSlitWidth_triggered()
{
    scene->setCurMode(GraphicsScene::InsertSlitWidth);
    qDebug() << "on_actionSlitWidth_triggered";
}

void MainWindow::on_actionRectangle_triggered()
{
    scene->setCurMode(GraphicsScene::InsertRectangle);
    qDebug() << "on_actionRectangle_triggered";
}

void MainWindow::on_actionAnyShape_triggered()
{
    scene->setCurMode(GraphicsScene::InsertAnyShape);
    qDebug() << "on_actionAnyShape_triggered";
}

void MainWindow::on_actionOccurrence_triggered()
{
    scene->setCurMode(GraphicsScene::InsertOccurance);
    qDebug() << "on_actionOccurrence_triggered";
}

void MainWindow::on_actionTextbox_triggered()
{
    scene->setCurMode(GraphicsScene::InsertTextBox);
    qDebug() << "on_actionTextbox_triggered";
}

void MainWindow::on_actionCross_triggered()
{
    scene->setCurMode(GraphicsScene::InsertCross);
    qDebug() << "on_actionCross_triggered";
}


// 将所有action重置
void MainWindow::resetActions()
{
    for (quint8 i = 0; i < editActionGroup->actions().count(); i++)
        editActionGroup->actions()[i]->setChecked(false);
}


void MainWindow::modeChanged(GraphicsScene::Mode lastMode, GraphicsScene::Mode curMode)
{
    if (curMode == GraphicsScene::MoveItem)
    {
        resetActions();
    }
}


// 当插入item后
void MainWindow::handleItemInserted(QGraphicsItem* const &insertedItem)
{

}

// 当删除item后
void MainWindow::handleItemDeleted(QGraphicsItem* const &deletedItem)
{

}

