#include "DefectWidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget),
    model(Q_NULLPTR)
{
    ui->setupUi(this);

    initModel();

    addedItems.clear();
    saveItems.clear();
}

DefectWidget::~DefectWidget()
{
    delete ui;
}


bool DefectWidget::hasAddedItem()
{
    return !(addedItems.isEmpty());
}


QVector<DefectWidget::ItemData> DefectWidget::getAddedItems()
{
    return addedItems;
}

void DefectWidget::clearAddedItems()
{
    addedItems.clear();
}


void DefectWidget::showRealInfo(QString info)
{
    ui->realInfoEdit->setText(info);
}


void DefectWidget::itemInserted(QGraphicsItem *item, QUuid uuid)
{
    ItemData data;
    data.item = item;
    data.uuid = uuid;
    addedItems.append(data);
}

void DefectWidget::updateItems(QVector<ItemData> items)
{
    addedItems.clear();
    saveItems.clear();
    saveItems.append(items);
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


