#include "ContactDialog.h"
#include "ui_ContactDialog.h"

ContactDialog::ContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContactDialog)
{
    ui->setupUi(this);
}

ContactDialog::~ContactDialog()
{
    delete ui;
}
