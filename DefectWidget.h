#pragma once

#include <QWidget>
#include <QDebug>

#include <QGraphicsItem>
#include <QUuid>
#include <QVector>


namespace Ui {
class DefectWidget;
}


class DefectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefectWidget(QWidget *parent = 0);
    ~DefectWidget();


    typedef struct {
        QUuid uuid;
        QGraphicsItem *item;
    }ItemData;

    bool hasAddedItem();
    QVector<ItemData> getAddedItems();
    void clearAddedItems();


public slots:
    void showRealInfo(QString info);
    void itemInserted(QGraphicsItem *item, QUuid uuid);
    void updateItems(QVector<ItemData> items);

private slots:



private:
    Ui::DefectWidget *ui;



    QVector<ItemData> addedItems;
    QVector<ItemData> saveItems;

signals:

};

