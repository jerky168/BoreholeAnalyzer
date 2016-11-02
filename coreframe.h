#ifndef COREFRAME_H
#define COREFRAME_H

#include <QFrame>

namespace Ui {
class CoreFrame;
}

class CoreFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CoreFrame(QWidget *parent = 0);
    ~CoreFrame();

private:
    Ui::CoreFrame *ui;
};

#endif // COREFRAME_H
