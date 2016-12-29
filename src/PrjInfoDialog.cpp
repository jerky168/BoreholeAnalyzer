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
    ui->diameterEdit->setText(QString::number(prjInfo.diameter, 'f', 3));
    ui->startDepthEdit->setText(QString::number(prjInfo.startHeight, 'f', 3));
    ui->endDepthEdit->setText(QString::number(prjInfo.endHeight, 'f', 3));
    ui->testTimeEdit->setText(prjInfo.projectTime);
    ui->testSiteEdit->setText(prjInfo.projectSite);
}


void PrjInfoDialog::clearPrjInfo()
{
    ui->nameEdit->clear();
    ui->numberEdit->clear();
    ui->diameterEdit->clear();
    ui->startDepthEdit->clear();
    ui->endDepthEdit->clear();
    ui->testTimeEdit->clear();
    ui->testSiteEdit->clear();
}


void PrjInfoDialog::on_buttonBox_accepted()
{
    DbHandler::PrjInfo info;
    bool ok = false;
    info.projectName = ui->nameEdit->text();
    info.projectSite = ui->testSiteEdit->text();
    info.projectTime = ui->testTimeEdit->text();
    info.orificeNumber = ui->numberEdit->text();
    info.diameter = ui->diameterEdit->text().toDouble(&ok);
    if (ui->diameterEdit->text().isEmpty() && !ok)
    {
        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
        return;
    }
    info.startHeight = ui->startDepthEdit->text().toDouble(&ok);
    if (ui->startDepthEdit->text().isEmpty() && !ok)
    {
        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
        return;
    }
    info.endHeight = ui->endDepthEdit->text().toDouble(&ok);
    if (ui->endDepthEdit->text().isEmpty() && !ok)
    {
        QMessageBox::warning(this, tr("Input error"), tr("Please input valid number!"));
        return;
    }
    emit savePrjInfo(info);
    clearPrjInfo();
    close();
}

void PrjInfoDialog::on_buttonBox_rejected()
{
    clearPrjInfo();
    close();
}
