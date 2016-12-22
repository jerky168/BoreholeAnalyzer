#include "ManualDialog.h"
#include "ui_ManualDialog.h"

ManualDialog::ManualDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualDialog)
{
    ui->setupUi(this);
}

ManualDialog::~ManualDialog()
{
    delete ui;
}
