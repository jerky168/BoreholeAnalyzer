#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QUndoCommand>
#include <QUndoStack>

#include "DbHandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void switchImage(quint16 index);


private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionTextbox_triggered();

private:
    Ui::MainWindow *ui;

    DbHandler *handler;
    QUndoStack *undosStack;

    typedef enum{
        IDLE,
        Opened
    }AppStatus;

signals:
    // 更新显示的照片
    void updatePixmap(QPixmap pixmap);
    void clearPixmap();

};

#endif // MAINWINDOW_H
