#pragma once

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class ShiftDialog;
}

class ShiftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShiftDialog(QWidget *parent = Q_NULLPTR);
    ~ShiftDialog();

    qreal getShiftAngle() {return angle;}

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ShiftDialog *ui;
    qreal angle;

signals:
    void sigShiftAngle(qreal angle);
};
