#pragma once

#include <QDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class ExportImageDialog;
}

class ExportImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportImageDialog(QWidget *parent = Q_NULLPTR);
    ~ExportImageDialog();


    enum ImageFormat{JPG, PNG, BMP};

    bool getForm();
    QString getPath();
    int getIndexForm();

private slots:
    void on_toolButton_clicked();

    void on_formCBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::ExportImageDialog *ui;
};

