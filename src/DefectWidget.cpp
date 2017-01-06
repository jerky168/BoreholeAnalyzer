#include "DefectWidget.h"
#include "ui_defectwidget.h"

DefectWidget::DefectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectWidget),
    model(Q_NULLPTR),
    headerView(Q_NULLPTR)
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
    model = new QStandardItemModel(0, 3, this);
    headerView = new QHeaderView(Qt::Horizontal);

    QStringList headers;
    headers << tr("Type") << tr("Data1") << tr("Data2");
    model->setHorizontalHeaderLabels(headers);
    ui->tableView->setModel(model);
}



void DefectWidget::showRealInfo(QString info)
{
    ui->realInfoEdit->setText(info);
}



void DefectWidget::updateTableData(QVector<GraphicsScene::TableData> tableDatas)
{
    model->removeRows(0, model->rowCount());
    for (int i = 0; i < tableDatas.count(); i++)
    {
        QList<QStandardItem *> items;
        QStandardItem *item;
//        item = new QStandardItem(tableDatas.at(i).isSaved);
//        items.append(item);
//        item = new QStandardItem(QString::number(ImageWidget::index+1));
//        items.append(item);
        item = new QStandardItem(tableDatas.at(i).type);
        items.append(item);
        QString data = tableDatas.at(i).data;
        item = new QStandardItem(data.section('\n', 0, 0));
        items.append(item);
        item = new QStandardItem(data.section('\n', 1).replace("\n", "  "));
        items.append(item);
        model->appendRow(items);

    }
    ui->tableView->resizeColumnsToContents();
    update();

    ui->deleteButton->setEnabled(true);
}



void DefectWidget::clearTableData()
{
    model->removeRows(0, model->rowCount());
    update();

    ui->deleteButton->setEnabled(false);
}


void DefectWidget::on_deleteButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row >= 0)
        emit deleteItem(row);
}
