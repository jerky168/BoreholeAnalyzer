#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}


void ImageWidget::reset()
{

}

void ImageWidget::on_lastButton_clicked()
{
    emit switchToLast();
}

void ImageWidget::on_nextButton_clicked()
{
    emit switchToNext();
}
