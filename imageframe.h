#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QFrame>

namespace Ui {
class ImageFrame;
}

class ImageFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ImageFrame(QWidget *parent = 0);
    ~ImageFrame();

private:
    Ui::ImageFrame *ui;
};

#endif // IMAGEFRAME_H
