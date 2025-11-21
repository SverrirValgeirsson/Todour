#ifndef TODOUNDO_H
#define TODOUNDO_H

#include <QUndoCommand>
#include "task.h"
#include "todotablemodel.h"
#include "taskset.h"

using namespace std;

//TODO: consider a masterclass doing all the common stuff??

class AddCommand : public QUndoCommand
{
public:
    explicit AddCommand(taskset* _list, task* _t, QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;
    int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
    task* _task;
    TodoTableModel* _model;
	taskset* tasklist;
};


class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(taskset* _list, QUuid index, QUndoCommand *parent = nullptr);
	~DeleteCommand();
    void undo() override;
    void redo() override;
	int id() const;
	bool mergeWith(const QUndoCommand *other);
	
protected:
	task* _task;
	TodoTableModel* _model;
	taskset* tasklist;
 	QUuid _tuid;


};


class EditCommand : public QUndoCommand
{
public:
    explicit EditCommand(taskset* _list, task* t, QString new_raw, QUndoCommand *parent = nullptr);
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
	taskset* tasklist;
    };


class CompleteCommand : public QUndoCommand
{
public:
    explicit CompleteCommand(taskset* _list, task* t, bool complete, QUndoCommand *parent = nullptr);
    explicit CompleteCommand(taskset* _list, task* t, QUndoCommand *parent = nullptr);
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
	taskset* tasklist;

};

class PriorityCommand : public QUndoCommand
{
public:
    explicit PriorityCommand(taskset* _list, task* t, QChar prio, QUndoCommand *parent = nullptr);
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
	taskset* tasklist;

};

class PostponeCommand : public EditCommand
{
public:
    explicit PostponeCommand(taskset* _list, task* t, QString _postp, QUndoCommand *parent = nullptr);
	~PostponeCommand();
	
private:

};



#endif // TODOUNDO_H
