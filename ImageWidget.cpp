#include "ImageWidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget),
    lastIndex(0),
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
    maxIndex = prjInfo.endHeight;
    index = 0;
    emit sigSwitchImage(index);

    ui->startHeightEdit->setText(QString::number(prjInfo.startHeight, 'f', 3).append("m"));
    ui->endHeightEdit->setText(QString::number(prjInfo.endHeight, 'f', 3).append("m"));
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
    ui->totalLengthEdit->setText(QString::number(prjInfo.endHeight - prjInfo.startHeight, 'f', 3).append("m"));
    ui->totalPartEdit->setText(QString::number(maxIndex+1));
}

void ImageWidget::clear()
{
    ui->startHeightEdit->clear();
    ui->endHeightEdit->clear();
    ui->currentDepthEdit->clear();
    ui->currentPartEdit->clear();
    ui->totalLengthEdit->clear();
    ui->totalPartEdit->clear();
}




quint16 ImageWidget::getIndex()
{
    return index;
}



quint16 ImageWidget::getMaxIndex()
{
    return maxIndex;
}


void ImageWidget::cancelSwitch()
{
    index = lastIndex;
}


void ImageWidget::on_lastButton_clicked()
{
    if (0 == index)
        return;

    lastIndex = index;
    index--;
    emit sigSwitchImage(index);
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}

void ImageWidget::on_nextButton_clicked()
{
    if (maxIndex == index)
        return;

    lastIndex = index;
    index++;
    emit sigSwitchImage(index);
    ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}


void ImageWidget::on_switchButton_clicked()
{
    bool ok = false;

    lastIndex = index;
    index = ui->switchEdit->text().toInt(&ok) - 1;
    if (ok && index > 0 && index <= maxIndex)
    {
        emit sigSwitchImage(index);
        ui->currentDepthEdit->setText(QString::number(index+1).append("m"));
        ui->currentPartEdit->setText(QString::number(index+1));
    }

    ui->switchEdit->clear();
}
