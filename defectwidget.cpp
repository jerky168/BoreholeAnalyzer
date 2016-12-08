#include "defectwidget.h"
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


void DefectWidget::showRealInfo(QString info)
{
    ui->realInfoEdit->setText(info);
}



void DefectWidget::on_clearButton_clicked()
{
    ui->descipEdit->clear();
}


void DefectWidget::on_addButton_clicked()
{
    emit addItemClicked();
    ui->realInfoEdit->clear();
    ui->descipEdit->clear();
}
