#include "defectwidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget)
{
    ui->setupUi(this);

//     QSqlQueryModel *model = new QSqlQueryModel();
//     model->setQuery("SELECT uuid, type, data FROM items");
//     model->setHeaderData(0, Qt::Horizontal, tr("uuid"));
//     model->setHeaderData(1, Qt::Horizontal, tr("type"));
//     model->setHeaderData(2, Qt::Horizontal, tr("data"));
//     ui->tableView->setModel(model);
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
