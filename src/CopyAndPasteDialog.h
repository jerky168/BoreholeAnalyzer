#ifndef COPYANDPASTEDIALOG_H
#define COPYANDPASTEDIALOG_H

#include <QDialog>

namespace Ui {
class CopyAndPasteDialog;
}

class CopyAndPasteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyAndPasteDialog(QWidget *parent = 0);
    ~CopyAndPasteDialog();

private:
    Ui::CopyAndPasteDialog *ui;
};

#endif // COPYANDPASTEDIALOG_H
