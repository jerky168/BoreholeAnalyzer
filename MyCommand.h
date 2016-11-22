#ifndef MYCOMMAND_H
#define MYCOMMAND_H

#include <QUndoCommand>

class MyCommand : public QUndoCommand
{
public:
    MyCommand();
    ~MyCommand();

    void redo();
    void undo();
<<<<<<< 756f4e16bab91d7d8128f07f75bc6438a70bf51a

=======
>>>>>>> 参考老版本的架构，继承Qt提供的基类，编写了不同功能的类，重写了view和scene的鼠标和滚轮事件。


};

#endif // MYCOMMAND_H
