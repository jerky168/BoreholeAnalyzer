#include "DefectWidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget),
    model(Q_NULLPTR)
{
    ui->setupUi(this);

    initModel();
}

DefectWidget::~DefectWidget()
{
    delete ui;
    delete model;
}

void DefectWidget::initModel()
{
    model = new QStandardItemModel(0, 4, this);
    model->setHeaderData(0, Qt::Horizontal, "页数");
    model->setHeaderData(1, Qt::Horizontal, "类型");
    model->setHeaderData(2, Qt::Horizontal, "是否保存");
    model->setHeaderData(3, Qt::Horizontal, "数据");
    ui->tableView->setModel(model);
}




void DefectWidget::showRealInfo(QString info)
{
    ui->realInfoEdit->setText(info);
}

void DefectWidget::updateTableData(QVector<GraphicsScene::TableData> tableDatas)
{
    model->clear();
    for (int i = 0; i < tableDatas.count(); i++)
    {
        QList<QStandardItem *> items;
        QStandardItem *item = new QStandardItem("N/A");
        items.append(item);
        item = new QStandardItem(tableDatas.at(i).type);
        items.append(item);
        item = new QStandardItem(tableDatas.at(i).isSaved);
        items.append(item);
        item = new QStandardItem(tableDatas.at(i).data);
        items.append(item);
        model->appendRow(items);
    }
    this->update();
}




