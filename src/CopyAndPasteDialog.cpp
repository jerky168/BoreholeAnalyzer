#include "CopyAndPasteDialog.h"
#include "ui_CopyAndPasteDialog.h"

CopyAndPasteDialog::CopyAndPasteDialog(qreal totalStart, qreal totalEnd, qreal currentStart, qreal currentEnd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyAndPasteDialog)
{
    ui->setupUi(this);

    ui->totalStartEdit->setText(QString::number(totalStart, 'f', 3) + " m");
    ui->totalEndEdit->setText(QString::number(totalEnd, 'f', 3) + " m");

    ui->currentStartEdit->setText(QString::number(currentStart, 'f', 3) + " m");
    ui->currentEndEdit->setText(QString::number(currentEnd, 'f', 3) + " m");

    ui->selectStartSBox->setMinimum(currentStart);
    ui->selectStartSBox->setMaximum(currentEnd);
    ui->selectEndSBox->setMinimum(currentStart);
    ui->selectEndSBox->setMaximum(currentEnd);
    ui->insertSBox->setMinimum(currentStart);
    ui->insertSBox->setMaximum(currentEnd);


    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

CopyAndPasteDialog::~CopyAndPasteDialog()
{
    delete ui;
}

QPointF CopyAndPasteDialog::getSection()
{
    return QPointF(ui->selectStartSBox->value(), ui->selectEndSBox->value());
}

qreal CopyAndPasteDialog::getDepth()
{
    return ui->insertSBox->value();
}


void CopyAndPasteDialog::on_selectStartSBox_valueChanged(double value)
{
    ui->selectEndSBox->setMinimum(value);
}

void CopyAndPasteDialog::on_buttonBox_accepted()
{
    if (ui->selectStartSBox->value() < ui->selectEndSBox->value())
    {
        QMessageBox messageBox(QMessageBox::Question,
                               tr("Confirm"),
                               tr("The section selected is: ")
                               + QString::number(ui->selectStartSBox->value(), 'f', 3) + "m-"
                               + QString::number(ui->selectEndSBox->value(), 'f', 3) + "m\n"
                               + tr("The depth to insert selected section is: ")
                               + QString::number(ui->insertSBox->value()) + "m\n"
                               + tr("Are you sure to proceed?"),
                               QMessageBox::Yes | QMessageBox::Cancel,
                               this);
        messageBox.button(QMessageBox::Yes)->setText(tr("Yes"));
        messageBox.button(QMessageBox::Cancel)->setText(tr("Cancel"));
        if (QMessageBox::Yes == messageBox.exec())
            accept();
    }
    else if (ui->selectStartSBox->value() == ui->selectEndSBox->value())
    {
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Note"),
                               tr("Selected section is empty."),
                               QMessageBox::Ok,
                               this);
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
    }
}
