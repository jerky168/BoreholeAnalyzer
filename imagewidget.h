#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

public slots:
    void reset();


private slots:
    void on_lastButton_clicked();
    void on_nextButton_clicked();

private:
    Ui::ImageWidget *ui;

signals:
    void lastImage();
    void nextImage();

};

#endif // IMAGEWIDGET_H
