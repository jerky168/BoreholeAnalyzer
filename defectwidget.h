#pragma once

#include <QWidget>
#include <QDebug>
#include <QSqlQueryModel>

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


private slots:
    void on_clearButton_clicked();

    void on_addButton_clicked();

private:
    Ui::DefectWidget *ui;

signals:
    void addItemClicked();

};

