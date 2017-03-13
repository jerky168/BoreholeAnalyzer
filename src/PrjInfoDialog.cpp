#include "PrjInfoDialog.h"
#include "ui_PrjInfoDialog.h"

PrjInfoDialog::PrjInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrjInfoDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}


PrjInfoDialog::~PrjInfoDialog()
{
    delete ui;
}


void PrjInfoDialog::updatePrjInfo(DbHandler::PrjInfo prjInfo)
{
    ui->nameEdit->setText(prjInfo.projectName);
    ui->numberEdit->setText(prjInfo.orificeNumber);
    ui->diameterSpinBox->setValue(prjInfo.diameter);
    ui->startDepthSpinBox->setValue(prjInfo.startHeight);
    ui->endDepthSpinBox->setValue(prjInfo.endHeight);
    ui->testTimeEdit->setText(prjInfo.projectTime);
    ui->testSiteEdit->setText(prjInfo.projectSite);

    ui->startDepthSpinBox->setMaximum(prjInfo.endHeight);
}


void PrjInfoDialog::clearPrjInfo()
{
    ui->nameEdit->clear();
    ui->numberEdit->clear();
    ui->diameterSpinBox->clear();
    ui->startDepthSpinBox->clear();
    ui->endDepthSpinBox->clear();
    ui->testTimeEdit->clear();
    ui->testSiteEdit->clear();
}


void PrjInfoDialog::on_buttonBox_accepted()
{
    DbHandler::PrjInfo info;
//    bool ok = false;
    info.projectName = ui->nameEdit->text();
    info.projectSite = ui->testSiteEdit->text();
    info.projectTime = ui->testTimeEdit->text();
    info.orificeNumber = ui->numberEdit->text();
    info.diameter = ui->diameterSpinBox->value();
//    if (ui->diameterEdit->text().isEmpty() && !ok)
//    {
//        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
//        return;
//    }
    info.startHeight = ui->startDepthSpinBox->value();
//    if (ui->startDepthEdit->text().isEmpty() && !ok)
//    {
//        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
//        return;
//    }
    info.endHeight = ui->endDepthSpinBox->value();
//    if (ui->endDepthEdit->text().isEmpty() && !ok)
//    {
//        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
//        return;
//    }



    emit savePrjInfo(info);
    QMessageBox::information(this, tr("Success"), tr("Save project information successfully."), tr("Ok"));
    clearPrjInfo();
    close();
}

void PrjInfoDialog::on_buttonBox_rejected()
{
    clearPrjInfo();
    close();
}
