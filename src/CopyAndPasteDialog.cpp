#include "CopyAndPasteDialog.h"
#include "ui_CopyAndPasteDialog.h"

CopyAndPasteDialog::CopyAndPasteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyAndPasteDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

CopyAndPasteDialog::~CopyAndPasteDialog()
{
    delete ui;
}
