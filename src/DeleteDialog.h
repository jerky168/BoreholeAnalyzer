#pragma once

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteDialog(qreal currentStart, qreal currentEnd, QWidget *parent = Q_NULLPTR);
    ~DeleteDialog();

public slots:


private slots:
    void on_deleteStartSBox_valueChanged(double value);
    void on_buttonBox_accepted();

private:
    Ui::DeleteDialog *ui;
};

