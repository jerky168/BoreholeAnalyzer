#include "PrjInfoDialog.h"
#include "ui_PrjInfoDialog.h"

PrjInfoDialog::PrjInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrjInfoDialog),
    hasChange(false)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确认");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}


PrjInfoDialog::~PrjInfoDialog()
{
    delete ui;
}


void PrjInfoDialog::updatePrjInfo(DbHandler::PrjInfo prjInfo)
{
    ui->numberEdit->setText(prjInfo.orificeNumber);
    ui->diameterEdit->setText(QString::number(prjInfo.diameter, 'f', 3));
    ui->startDepthEdit->setText(QString::number(prjInfo.startHeight, 'f', 3));
    ui->endDepthEdit->setText(QString::number(prjInfo.endHeight, 'f', 3));
    ui->testTimeEdit->setText(prjInfo.projectTime);
    ui->testSiteEdit->setText(prjInfo.projectSite);
}


void PrjInfoDialog::clearPrjInfo()
{
    ui->numberEdit->clear();
    ui->diameterEdit->clear();
    ui->startDepthEdit->clear();
    ui->endDepthEdit->clear();
    ui->testTimeEdit->clear();
    ui->testSiteEdit->clear();
}


void PrjInfoDialog::on_buttonBox_accepted()
{
    if (hasChange)
    {

    }
    else
    {
        this->accept();
    }
}

void PrjInfoDialog::on_buttonBox_rejected()
{
    if (hasChange)
    {

    }
    else
    {
        this->reject();
    }
}
