#include "imageframe.h"
#include "ui_imageframe.h"

ImageFrame::ImageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageFrame)
{
    ui->setupUi(this);
}

ImageFrame::~ImageFrame()
{
    delete ui;
}
