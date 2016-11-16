#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),
    index(0),
    maxIndex(0)
{
    ui->setupUi(this);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}


void ImageWidget::updatePrjInfo(DbHandler::PrjInfo prjInfo)
{
    maxIndex = prjInfo.endHeight / 10000;
    index = 0;
    emit sigSwitchImage(index);

    ui->startHeightEdit->setText(QString::number(prjInfo.startHeight / 10000.0));
    ui->endHeightEdit->setText(QString::number(prjInfo.endHeight / 10000.0));
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
    ui->partLengthEdit->setText(QString::number(1).append("m"));
    ui->totalLengthEdit->setText(QString::number((prjInfo.endHeight - prjInfo.startHeight) / 10000.0).append("m"));
    ui->totalPartEdit->setText(QString::number(maxIndex+1));
}


void ImageWidget::clear()
{
    ui->startHeightEdit->clear();
    ui->endHeightEdit->clear();
    ui->currentDepthEdit->clear();
    ui->currentPartEdit->clear();
    ui->partLengthEdit->clear();
    ui->totalLengthEdit->clear();
    ui->totalPartEdit->clear();
}



void ImageWidget::on_lastButton_clicked()
{
    if (0 == index)
        return;

    index--;
    emit sigSwitchImage(index);
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}

void ImageWidget::on_nextButton_clicked()
{
    if (maxIndex == index)
        return;

    index++;
    emit sigSwitchImage(index);
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}



