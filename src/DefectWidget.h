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


private:
    Ui::DefectWidget *ui;

    QStandardItemModel *model;
    QHeaderView *headerView;

    void initModel();

private slots:
    void on_deleteButton_clicked();


signals:

};

