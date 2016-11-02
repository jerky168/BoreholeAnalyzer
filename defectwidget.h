#ifndef DEFECTWIDGET_H
#define DEFECTWIDGET_H

#include <QWidget>

namespace Ui {
class DefectWidget;
}

class DefectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DefectWidget(QWidget *parent = 0);
    ~DefectWidget();

private:
    Ui::DefectWidget *ui;
};

#endif // DEFECTWIDGET_H
