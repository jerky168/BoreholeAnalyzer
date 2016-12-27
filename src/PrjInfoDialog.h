#pragma once

#include <QDialog>

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

    void updatePrjInfo(DbHandler::PrjInfo prjInfo);
    void clearPrjInfo();


private:
    Ui::PrjInfoDialog *ui;
};

