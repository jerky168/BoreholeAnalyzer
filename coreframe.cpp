#include "coreframe.h"
#include "ui_coreframe.h"

CoreFrame::CoreFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoreFrame)
{
    ui->setupUi(this);
}

CoreFrame::~CoreFrame()
{
    delete ui;
}
