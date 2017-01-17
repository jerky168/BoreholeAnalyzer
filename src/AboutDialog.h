#pragma once

#include <QDialog>
#include <QPushButton>
#include "app.h"
#include <QFile>
#include <QDebug>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

