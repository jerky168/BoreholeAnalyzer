#include "TextInputDialog.h"
#include "ui_TextInputDialog.h"

TextInputDialog::TextInputDialog(QString text, qint32 fontSize, Color color, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);
    setText(text);
    setFontSize(fontSize);
    setColor(color);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    QPixmap pix(16, 16);
    QPainter painter(&pix);
    painter.fillRect(0, 0, 16, 16, Qt::black);
    ui->colorCBox->setItemIcon(0, QIcon(pix));
    painter.fillRect(0, 0, 16, 16, Qt::white);
    ui->colorCBox->setItemIcon(1, QIcon(pix));
    painter.fillRect(0, 0, 16, 16, Qt::red);
    ui->colorCBox->setItemIcon(2, QIcon(pix));
    painter.fillRect(0, 0, 16, 16, Qt::green);
    ui->colorCBox->setItemIcon(3, QIcon(pix));
    painter.fillRect(0, 0, 16, 16, Qt::blue);
    ui->colorCBox->setItemIcon(4, QIcon(pix));
    painter.fillRect(0, 0, 16, 16, Qt::yellow);
    ui->colorCBox->setItemIcon(5, QIcon(pix));
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

void TextInputDialog::on_buttonBox_accepted()
{
    if (ui->textEdit->toPlainText().isEmpty())
    {
        QMessageBox messageBox(QMessageBox::Warning, tr("Warning"), tr("Input textbox is empty."), QMessageBox::Ok, this);
        messageBox.button(QMessageBox::Ok)->setText(tr("Ok"));
        messageBox.exec();
        return;
    }
    accept();
}


QString TextInputDialog::getText()
{
    return ui->textEdit->toPlainText();
}

void TextInputDialog::setText(QString text)
{
    ui->textEdit->setText(text);
}

qint32 TextInputDialog::getFontSize()
{
    return ui->sizeSBox->value();
}

void TextInputDialog::setFontSize(qint32 fontSize)
{
    ui->sizeSBox->setValue(fontSize);
}

TextInputDialog::Color TextInputDialog::getColor()
{
    return (Color)ui->colorCBox->currentIndex();
}

void TextInputDialog::setColor(Color color)
{
    int index = color;
    ui->colorCBox->setCurrentIndex(index);
}

