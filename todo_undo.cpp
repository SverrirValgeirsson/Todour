#include "todo_undo.h"
#include "def.h"
#include <QDebug>

AddCommand::AddCommand(TodoTableModel* model, task* _t, QUndoCommand *parent)
    : QUndoCommand(parent), _model(model)
/* */
{
	_task = _t;
	setText("New task");
}

AddCommand::~AddCommand()
/* */
{}

void AddCommand::undo()
/* undo() of addCommand is a remove
*/
{
	_model->removeTask(_task->getTuid());
}

void AddCommand::redo()
/* redo() add is adding again
*/
{
	_model->addTask(_task);
}

int AddCommand::id() const
{return -1;}

bool AddCommand::mergeWith(const QUndoCommand *other)
{Q_UNUSED(other);
return false;}


// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

DeleteCommand::DeleteCommand(TodoTableModel* model, QUuid index, QUndoCommand *parent)
    : QUndoCommand(parent), _model(model), _tuid(index)
/*	_task = is initialised in the redo */
{
	setText("Delete");
}

DeleteCommand::~DeleteCommand()
/* */
{}

void DeleteCommand::undo()
/* UNDO DELETE means we have to add a task in the list.
*/
{
	_model->addTask(_task);
}

void DeleteCommand::redo()
/* */
{
	// as far as I know, <vector> doesn't actually delete the object.
	//_task=_model->getTask(_tuid);
	_task = _model->removeTask(_tuid);
	//qDebug()<<"DeleteCommand::redo()  _task="<<_task->toString()<<endline;
}

int DeleteCommand::id() const
/* */
{
	return -1;
}

bool DeleteCommand::mergeWith(const QUndoCommand *other)
/* */
{
	Q_UNUSED(other);
	return false;
}


// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

EditCommand::EditCommand(TodoTableModel* model, task* t, QString new_raw, QUndoCommand *parent)
    :QUndoCommand(parent), _task(t), _model(model)
/* */
{
	_old_raw = t->getRaw();
	 _new_raw = new_raw;

	setText("Edit");
}

EditCommand::~EditCommand()
/* */
{}

void EditCommand::undo()
/* */
{
	_task->setRaw(_old_raw);
}

void EditCommand::redo()
/* */
{
	_task->setRaw(_new_raw);
}

int EditCommand::id() const
/**/
{
	return -1;
}

bool EditCommand::mergeWith(const QUndoCommand *other)
/* */
{
	Q_UNUSED(other);
	return false;
}

// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

CompleteCommand::CompleteCommand(TodoTableModel* model, task* t, bool complete, QUndoCommand *parent)
    :QUndoCommand(parent), _task(t),_complete(complete), _model(model)
/* */
{
	setText("Complete");
	rec_task = nullptr;
}

CompleteCommand::CompleteCommand(TodoTableModel* model, task* t, QUndoCommand *parent)
    :QUndoCommand(parent), _task(t), _model(model)
/* */
{
	setText("Complete");
	rec_task = nullptr;
	_complete=!t->isComplete();
}

CompleteCommand::~CompleteCommand()
/* #TODO  check*/
{
}

void CompleteCommand::undo()
/* */
{
	_task->setComplete(!_complete);
	if (rec_task != nullptr)
			_model->removeTask(rec_task->getTuid());
	
}

void CompleteCommand::redo()
/* */
{
	rec_task=_task->setComplete(_complete);
	if (rec_task != nullptr)
			_model->addTask(rec_task);
}

int CompleteCommand::id() const
/* */
{
	return -1;
}

bool CompleteCommand::mergeWith(const QUndoCommand *other)
/* */
{
	Q_UNUSED(other);
	return false;
}

// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

PriorityCommand::PriorityCommand(TodoTableModel* model, task* t, QChar prio, QUndoCommand *parent)
    :QUndoCommand(parent), _task(t), _priority(prio), _model(model)
/* */
{
	setText("Priority");
	_p_priority = _task->getPriority();
}

PriorityCommand::~PriorityCommand()
/* #TODO  check*/
{}

void PriorityCommand::undo()
/* */
{
	_task->setPriority(_p_priority);
}

void PriorityCommand::redo()
/* */
{
	_task->setPriority(_priority);
}

int PriorityCommand::id() const
/* */
{
	return -1;
}

bool PriorityCommand::mergeWith(const QUndoCommand *other)
/* */
{
	Q_UNUSED(other);
	return false;
}


// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

PostponeCommand::PostponeCommand(TodoTableModel* model, task* t, QString postp, QUndoCommand *parent)
    :EditCommand(model, t,"",parent)
/* */
{
//	QSettings
	setText("Postpone");
	_new_raw = _old_raw + " " + postp;
}

PostponeCommand::~PostponeCommand()
/* #TODO  check*/
{}


