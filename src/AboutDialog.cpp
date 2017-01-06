#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QPushButton>
#include "app.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(App_Name);
    ui->contentLabel->setText(App_Name + "\nV" + App_Version);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
