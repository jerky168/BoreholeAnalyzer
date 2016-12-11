#include "defectwidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget)
{
    ui->setupUi(this);

    items->clear();
}

DefectWidget::~DefectWidget()
{
    delete ui;
}


void DefectWidget::saveAllItems()
{

}



void DefectWidget::showRealInfo(QString info)
{
    ui->realInfoEdit->setText(info);
}


void DefectWidget::on_clearButton_clicked()
{
    ui->descipEdit->clear();
}


void DefectWidget::itemInserted(QGraphicsItem *item, QUuid uuid)
{
    qDebug() << item->type() << uuid.toString();
}
