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
};

#endif // MYCOMMAND_H
