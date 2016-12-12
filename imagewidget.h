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

    void updatePrjInfo(DbHandler::PrjInfo prjInfo);
    void clear();

    quint16 getIndex();
    void cancelSwitch();

private slots:
    void on_lastButton_clicked();
    void on_nextButton_clicked();

    void on_switchButton_clicked();

private:
    Ui::ImageWidget *ui;

    quint16 lastIndex;
    quint16 index;
    quint16 maxIndex;


signals:
    void sigSwitchImage(quint16 index);

};

