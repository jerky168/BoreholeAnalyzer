#pragma once

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>
#include "DbHandler.h"

namespace Ui {
class PrjInfoDialog;
}

class PrjInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrjInfoDialog(QWidget *parent = Q_NULLPTR);
    ~PrjInfoDialog();


public slots:
    void updatePrjInfo(DbHandler::PrjInfo prjInfo);
    void clearPrjInfo();


private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::PrjInfoDialog *ui;

signals:
    void savePrjInfo(DbHandler::PrjInfo);
};

