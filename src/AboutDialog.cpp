#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(App_Name_CN);
    ui->contentLabel->setText(App_Name_CN + "\nv" + App_Version);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));

    QFile file("changelog");
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        ui->plainTextEdit->setPlainText(QString(file.readAll()));
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
