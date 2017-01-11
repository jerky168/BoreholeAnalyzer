#include "ImageWidget.h"
#include "ui_imagewidget.h"


qint32 ImageWidget::lastIndex = 0;
qint32 ImageWidget::index = 0;
qint32 ImageWidget::maxIndex = 0;



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


void ImageWidget::updatePrjInfo(DbHandler::PrjInfo prjInfo)
{
    maxIndex = qCeil(prjInfo.endHeight) - 1;
    index = 0;
    lastIndex = 0;
    emit sigSwitchImage(index);

    ui->startHeightEdit->setText(QString::number(prjInfo.startHeight, 'f', 3).append(" m"));
    ui->endHeightEdit->setText(QString::number(prjInfo.endHeight, 'f', 3).append(" m"));
    ui->currentDepthEdit->setText(QString::number(index+1).append(" m"));
    ui->currentPartEdit->setText(QString::number(index+1));
    ui->totalLengthEdit->setText(QString::number(prjInfo.endHeight - prjInfo.startHeight, 'f', 3).append(" m"));
    ui->totalPartEdit->setText(QString::number(qCeil(prjInfo.endHeight)));

    ui->switchEdit->setEnabled(true);
    ui->switchButton->setEnabled(true);
    ui->lastButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
}

void ImageWidget::clearPrjInfo()
{
    maxIndex = 0;
    index = 0;
    lastIndex = 0;

    ui->startHeightEdit->clear();
    ui->endHeightEdit->clear();
    ui->currentDepthEdit->clear();
    ui->currentPartEdit->clear();
    ui->totalLengthEdit->clear();
    ui->totalPartEdit->clear();

    ui->switchEdit->setEnabled(false);
    ui->switchButton->setEnabled(false);
    ui->lastButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
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
    ui->currentDepthEdit->setText(QString::number(index+1).append(" m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}

void ImageWidget::on_nextButton_clicked()
{
    if (maxIndex == index)
        return;

    lastIndex = index;
    index++;
    emit sigSwitchImage(index);
    ui->currentDepthEdit->setText(QString::number(index+1).append(" m"));
    ui->currentPartEdit->setText(QString::number(index+1));
}

void ImageWidget::on_switchButton_clicked()
{
    bool ok = false;

    lastIndex = index;
    index = ui->switchEdit->text().toInt(&ok) - 1;

    if (ok && index >= 0 && index <= maxIndex)
    {
        emit sigSwitchImage(index);
        ui->currentDepthEdit->setText(QString::number(index+1).append(" m"));
        ui->currentPartEdit->setText(QString::number(index+1));
    }
    else
    {
        index = lastIndex;
        QMessageBox messageBox(QMessageBox::Warning,
                               tr("Input error"),
                               tr("Please input a valid index! ") + "(1-" + QString::number(maxIndex+1) + ")",
                               QMessageBox::Ok,
                               this
                               );
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
    }

    ui->switchEdit->clear();
}


void ImageWidget::setIndex(qint32 newIndex)
{
    if (newIndex < 0 || newIndex > maxIndex)
        return;

    index = newIndex;
    lastIndex = newIndex;

    ui->currentDepthEdit->setText(QString::number(index+1).append(" m"));
    ui->currentPartEdit->setText(QString::number(index+1));

    emit sigSwitchImage(index);
}

