#include "defectwidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget)
{
    ui->setupUi(this);

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


void DefectWidget::on_clearButton_clicked()
{
    ui->descipEdit->clear();
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


