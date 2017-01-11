#pragma once

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class CopyAndPasteDialog;
}

class CopyAndPasteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyAndPasteDialog(qreal totalStart, qreal totalEnd, qreal currentStart, qreal currentEnd, QWidget *parent = Q_NULLPTR);
    ~CopyAndPasteDialog();

    QPointF getSection();
    qreal getDepth();


private slots:
    void on_selectStartSBox_valueChanged(double arg1);

    void on_buttonBox_accepted();

private:
    Ui::CopyAndPasteDialog *ui;
};


