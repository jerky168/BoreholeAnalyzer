#include "ExportImageDialog.h"
#include "ui_ExportImageDialog.h"


ExportImageDialog::ExportImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportImageDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

ExportImageDialog::~ExportImageDialog()
{
    delete ui;
}

void ExportImageDialog::on_toolButton_clicked()
{
    if (getForm())
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Export image"), tr(""), tr("Images files(*.jpg *.png *.bmp);;All files (*.*)"));
        ui->pathEdit->setText(filename);
    }
    else
    {
        QString folderPath = QFileDialog::getExistingDirectory(this, tr("Export all images"), QDir::homePath());
        ui->pathEdit->setText(folderPath);
    }
}

void ExportImageDialog::on_formCBox_currentIndexChanged(int index)
{
    ui->pathEdit->clear();
    if (0 == index)
    {
        ui->filenameLabel->setEnabled(false);
        ui->prefixEdit->setEnabled(false);
        ui->indexCBox->setEnabled(false);
        ui->formatCBox->setEnabled(false);
    }
    else
    {
        ui->filenameLabel->setEnabled(true);
        ui->prefixEdit->setEnabled(true);
        ui->indexCBox->setEnabled(true);
        ui->formatCBox->setEnabled(true);
    }
}

bool ExportImageDialog::getForm()
{
    return (0 == ui->formCBox->currentIndex());
}

QString ExportImageDialog::getPath()
{
    if (getForm())
    {
        return ui->pathEdit->text();
    }
    else
    {
        QString filename = ui->prefixEdit->text();
        if (!filename.isEmpty())
            filename.append("_");
        filename.append("to-be-filled");
        switch (ui->formatCBox->currentIndex()) {
        case JPG:
            filename.append(".jpg");
            break;
        case PNG:
            filename.append(".png");
            break;
        case BMP:
            filename.append(".bmp");
            break;
        default:
            break;
        }
        return QDir(ui->pathEdit->text()).filePath(filename);
    }
}

int ExportImageDialog::getIndexForm()
{
    return ui->indexCBox->currentIndex();
}


void ExportImageDialog::on_buttonBox_accepted()
{
    if (ui->pathEdit->text().isEmpty())
    {
        QMessageBox messageBox(QMessageBox::Warning, tr("Warning"), tr("Path textbox is empty!"), QMessageBox::Ok, this);
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
        return;
    }

    accept();
}
