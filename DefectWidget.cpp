#include "DefectWidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget)
{
    ui->setupUi(this);
}

DefectWidget::~DefectWidget()
{
    delete ui;
}
