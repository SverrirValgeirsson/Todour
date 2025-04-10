#ifndef TODOUNDO_H
#define TODOUNDO_H

#include <QUndoCommand>
#include "task.h"

class AddCommand : public QUndoCommand
{
public:
    AddCommand( task _t, QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
//    DiagramItem *myDiagramItem;
    task _task;
//    QPointF initialPosition;
};


class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(task _t, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
//    DiagramItem *myDiagramItem;
//    QGraphicsScene *myGraphicsScene;
		task _task;
};


class EditCommand : public QUndoCommand
{
public:
    explicit EditCommand(task _old, task _new, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
//    DiagramItem *myDiagramItem;
//    QGraphicsScene *myGraphicsScene;
	task _old;
	task _new;
};


#endif // TODOUNDO_H
