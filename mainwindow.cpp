#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->action2DView);
    actionGroup->addAction(ui->action3DView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
