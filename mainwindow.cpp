#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //make the 2D view and 3D view exclusive
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->action2DView);
    actionGroup->addAction(ui->action3DView);

    //switch 2D view and 3D view
    connect(ui->action2DView, &QAction::triggered, [this](bool checked) {ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->action3DView, &QAction::triggered, [this](bool checked) {ui->stackedWidget->setCurrentIndex(1);});

}

MainWindow::~MainWindow()
{
    delete ui;
}
