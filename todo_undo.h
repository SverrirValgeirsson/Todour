#ifndef TODOUNDO_H
#define TODOUNDO_H

#include <QUndoCommand>
#include "task.h"
#include "todotablemodel.h"

using namespace std;

class AddCommand : public QUndoCommand
{
public:
    explicit AddCommand(TodoTableModel* model, task* _t, QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;
    int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
    task* _task;
    TodoTableModel* _model;

};


class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(TodoTableModel* model, QUuid index, QUndoCommand *parent = nullptr);
	~DeleteCommand();
    void undo() override;
    void redo() override;
	int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
	task* _task;
	TodoTableModel* _model;
 	QUuid _tuid;


};


class EditCommand : public QUndoCommand
{
public:
    explicit EditCommand(TodoTableModel* model, task* t, QString new_raw, QUndoCommand *parent = nullptr);
	~EditCommand();
    void undo() override;
    void redo() override;
	int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
	task* _task;
	QString _old_raw;
	QString _new_raw;
	TodoTableModel* _model;
    };


class CompleteCommand : public QUndoCommand
{
public:
    explicit CompleteCommand(TodoTableModel* model, task* t, bool complete, QUndoCommand *parent = nullptr);
    explicit CompleteCommand(TodoTableModel* model, task* t, QUndoCommand *parent = nullptr);
	~CompleteCommand();
    void undo() override;
    void redo() override;
	int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
	task* _task;
	task* rec_task;
	bool _complete;
	TodoTableModel* _model;

};

class PriorityCommand : public QUndoCommand
{
public:
    explicit PriorityCommand(TodoTableModel* model, task* t, QChar prio, QUndoCommand *parent = nullptr);
	~PriorityCommand();
    void undo() override;
    void redo() override;
	int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
	task* _task;
	QChar _priority;
	QChar _p_priority;
	TodoTableModel* _model;

};

class PostponeCommand : public EditCommand
{
public:
    explicit PostponeCommand(TodoTableModel* model, task* t, QString _postp, QUndoCommand *parent = nullptr);
	~PostponeCommand();
	
private:

};



#endif // TODOUNDO_H
