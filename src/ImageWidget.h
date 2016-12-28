#pragma once

#include <QWidget>
#include "DbHandler.h"

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

    void cancelSwitch();


    static quint16 lastIndex, index, maxIndex;


public slots:
    void updatePrjInfo(DbHandler::PrjInfo prjInfo);
    void clearPrjInfo();


private slots:
    void on_lastButton_clicked();
    void on_nextButton_clicked();

    void on_switchButton_clicked();

private:
    Ui::ImageWidget *ui;



signals:
    void sigSwitchImage(quint16 index);

};

