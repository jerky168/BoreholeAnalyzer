#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene())
{
    ui->setupUi(this);

    // QGraphicsScene
    ui->graphicsView->setScene(scene);
    QPixmap pixmap;
    pixmapItem = scene->addPixmap(pixmap);

    //add the toobar and dock windows to menu view
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetImage->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetDefect->toggleViewAction());

    //make the 2D view and 3D view exclusive
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->action2DView);
    actionGroup->addAction(ui->action3DView);

    //switch 2D view and 3D view
    connect(ui->action2DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->action3DView, &QAction::triggered, [this](bool checked) {if (checked) ui->stackedWidget->setCurrentIndex(1);});


    // switch images
    QObject::connect(ui->imageWidget, SIGNAL(sigSwitchImage(quint16)), this, SLOT(switchImage(quint16)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开工程文件", QDir::homePath(), "工程文件 (*.ylink)");
    handler = new DbHandler(filename, this);
    DbHandler::PrjInfo prjInfo = handler->getPrjInfo();
    ui->imageWidget->updatePrjInfo(prjInfo);

    ui->actionClose->setEnabled(true);
}


void MainWindow::on_actionClose_triggered()
{
    QPixmap pixmap;
    pixmapItem->setPixmap(pixmap);
    ui->imageWidget->clear();

    ui->actionClose->setEnabled(false);
}




void MainWindow::switchImage(quint16 index)
{
    DbHandler::BigImage bigImage = handler->getBigImage(index);
    pixmapItem->setPixmap(bigImage.pixmap);
}

