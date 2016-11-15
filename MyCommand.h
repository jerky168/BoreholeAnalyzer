#ifndef MYCOMMAND_H
#define MYCOMMAND_H


class MyCommand : public QUndoCommand
{
public:
    MyCommand();
    ~MyCommand();

    redo();
    undo();



};

#endif // MYCOMMAND_H
