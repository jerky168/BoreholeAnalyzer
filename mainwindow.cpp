#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    connect(ui->imageWidget, SIGNAL(switchToLast()), this, SLOT());
    connect(ui->imageWidget, SIGNAL(switchToNext()), this, SLOT());
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

    DbHandler::BigImage bigImage = handler->getBigImage(0);

}
