#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QPainter>
#include <QDebug>

namespace Ui {
class TextInputDialog;
}

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:
    enum Color {Black, White, Red, Green, Blue, Yellow};

    explicit TextInputDialog(QString text, qint32 fontSize, Color color, QWidget *parent = Q_NULLPTR);
    ~TextInputDialog();

    QString getText();
    void setText(QString text);
    qint32 getFontSize();
    void setFontSize(qint32 fontSize);
    Color getColor();
    void setColor(Color color);

private:
    Ui::TextInputDialog *ui;

private slots:
    void on_buttonBox_accepted();
};

