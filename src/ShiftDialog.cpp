#include "ShiftDialog.h"
#include "ui_ShiftDialog.h"

ShiftDialog::ShiftDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShiftDialog),
    angle(0.0)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

ShiftDialog::~ShiftDialog()
{
    delete ui;
}


void ShiftDialog::on_buttonBox_accepted()
{
    bool ok = false;
    angle = ui->lineEdit->text().toDouble(&ok);
    if (!ok || angle > 180.0 || angle < -180.0)
    {
        QMessageBox messageBox(QMessageBox::Warning, tr("Input error"), tr("Please input a valid angle!"), QMessageBox::Ok, this);
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
    }
    else
    {
        accept();
    }
}
