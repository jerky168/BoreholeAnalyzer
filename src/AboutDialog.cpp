#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(App_Name_CN);
    ui->contentLabel->setText(App_Name_CN + "\nV" + App_Version);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
