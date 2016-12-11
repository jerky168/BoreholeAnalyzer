#pragma once

#include <QWidget>
#include <QDebug>
#include <QSqlQueryModel>

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

    void saveAllItems();


    typedef struct {
        bool hasSaved;
        QUuid uuid;
        QGraphicsItem *item;
    }itemData;


public slots:
    void showRealInfo(QString info);


private slots:
    void on_clearButton_clicked();
    void itemInserted(QGraphicsItem *item, QUuid uuid);

private:
    Ui::DefectWidget *ui;

    QVector<itemData> items;

signals:

};

