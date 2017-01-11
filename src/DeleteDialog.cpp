#include "DeleteDialog.h"
#include "ui_DeleteDialog.h"

DeleteDialog::DeleteDialog(qreal totalStart, qreal totalEnd, qreal currentStart, qreal currentEnd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);

    ui->totalStartEdit->setText(QString::number(totalStart, 'f', 3) + " m");
    ui->totalEndEdit->setText(QString::number(totalEnd, 'f', 3) + " m");

    ui->currentStartEdit->setText(QString::number(currentStart, 'f', 3) + " m");
    ui->currentEndEdit->setText(QString::number(currentEnd, 'f', 3) + " m");

    ui->deleteStartSBox->setMinimum(currentStart);
    ui->deleteStartSBox->setMaximum(currentEnd);
    ui->deleteEndSBox->setMinimum(currentStart);
    ui->deleteEndSBox->setMaximum(currentEnd);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}


QPointF DeleteDialog::getSection()
{
    return QPointF(ui->deleteStartSBox->value(), ui->deleteEndSBox->value());
}


void DeleteDialog::on_deleteStartSBox_valueChanged(double value)
{
    ui->deleteEndSBox->setMinimum(value);
}

void DeleteDialog::on_buttonBox_accepted()
{
    if (ui->deleteStartSBox->value() < ui->deleteEndSBox->value())
    {
        QMessageBox messageBox(QMessageBox::Question,
                               tr("Confirm"),
                               tr("The section to be deleted is: ")
                               + QString::number(ui->deleteStartSBox->value(), 'f', 3) + "m-"
                               + QString::number(ui->deleteEndSBox->value(), 'f', 3) + "m\n"
                               + tr("Are you sure to delete?"),
                               QMessageBox::Yes | QMessageBox::Cancel,
                               this);
        messageBox.button(QMessageBox::Yes)->setText(tr("Yes"));
        messageBox.button(QMessageBox::Cancel)->setText(tr("Cancel"));
        if (QMessageBox::Yes == messageBox.exec())
            accept();
    }
    else if (ui->deleteStartSBox->value() == ui->deleteEndSBox->value())
    {
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Note"),
                               tr("Delete section is empty."),
                               QMessageBox::Ok,
                               this);
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
    }
}
