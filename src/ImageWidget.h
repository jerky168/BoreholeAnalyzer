#pragma once


#include <QWidget>
#include <QMessageBox>
#include "DbHandler.h"
#include <QtMath>

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


    static qint32 lastIndex, index, maxIndex;


    static bool isStart() {return (index == 0);}
    static bool isEnd() {return (index == maxIndex);}

    void setIndex(qint32 newIndex);


public slots:
    void updatePrjInfo(DbHandler::PrjInfo prjInfo);
    void updateStartDepth(DbHandler::PrjInfo prjInfo);
    void clearPrjInfo();


private slots:
    void on_lastButton_clicked();
    void on_nextButton_clicked();

    void on_switchButton_clicked();

private:
    Ui::ImageWidget *ui;


signals:
    void sigSwitchImage(qint32 index);

};

