#include "PrjInfoDialog.h"
#include "ui_PrjInfoDialog.h"

PrjInfoDialog::PrjInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrjInfoDialog)
{
    ui->setupUi(this);
}


PrjInfoDialog::~PrjInfoDialog()
{
    delete ui;
}


void PrjInfoDialog::updatePrjInfo(DbHandler::PrjInfo prjInfo)
{
    ui->numberEdit->clear();
    ui->diameterEdit->clear();
    ui->startDepthEdit->setText(QString::number(prjInfo.startHeight, 'f', 3));
    ui->endDepthEdit->setText(QString::number(prjInfo.endHeight, 'f', 3));
    ui->testTimeEdit->clear();
    ui->endDepthEdit->clear();
}


void PrjInfoDialog::clearPrjInfo()
{
    ui->numberEdit->clear();
    ui->diameterEdit->clear();
    ui->startDepthEdit->clear();
    ui->endDepthEdit->clear();
    ui->testTimeEdit->clear();
    ui->endDepthEdit->clear();
}
