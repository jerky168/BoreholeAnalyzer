#pragma once

#include <QWidget>
#include <QDebug>

#include <QGraphicsItem>
#include <QUuid>
#include <QVector>

#include <QHeaderView>
#include <QStandardItemModel>
#include "ImageWidget.h"
#include "GraphicsScene.h"


namespace Ui {
class DefectWidget;
}

class DefectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefectWidget(QWidget *parent = 0);
    ~DefectWidget();


public slots:
    void showRealInfo(QString info);
    void updateTableData(QVector<GraphicsScene::TableData> tableDatas);
    void clearTableData();

private:
    Ui::DefectWidget *ui;

    QStandardItemModel *model;
    QHeaderView *headerView;

    QVector<GraphicsScene::TableData> datas;

    void initModel();

private slots:
    void on_deleteButton_clicked();

    void itemChanged(QStandardItem *item);

signals:
    void deleteItem(QUuid uuid);
    void updateItemRemark(QUuid uuid, QString remark);

};

