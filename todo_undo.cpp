#include "todo_undo.h"
#include "def.h"
#include <QDebug>

AddCommand::AddCommand(TodoTableModel* model, task* _t, QUndoCommand *parent)
    : QUndoCommand(parent), _model(model)
{
    _task = new task(_t);
	setText("New task");
}

AddCommand::~AddCommand(){}

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
	//_taskset->push_back(_task);
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
{
//	_task = is initialised in the redo
	setText("Delete");
}

DeleteCommand::~DeleteCommand(){}

void DeleteCommand::undo()
/* UNDO DELETE means we have to add a task in the list.
*/
{
	_model->addTask(_task);

}

void DeleteCommand::redo()
{
	_task=new task(_model->getTask(_tuid));
	_model->removeTask(_tuid);
}

int DeleteCommand::id() const
{return -1;}

bool DeleteCommand::mergeWith(const QUndoCommand *other)
{
Q_UNUSED(other);
return false;}


// §§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

EditCommand::EditCommand(TodoTableModel* model, task* t, QString new_raw, QUndoCommand *parent)
    :QUndoCommand(parent), _task(t), _model(model)
{
	_old_raw = t->getRaw();
	 _new_raw = new_raw;

	setText("Edit");
}
EditCommand::~EditCommand()
{}

void EditCommand::undo()
{
	_task->setRaw(_old_raw);
	_model->refresh();
}

void EditCommand::redo()
{
	_task->setRaw(_new_raw);
	_model->refresh();
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
{

	setText("Complete");
}
CompleteCommand::~CompleteCommand()
{}

void CompleteCommand::undo()
{
	_task->setComplete(!_complete);
	_model->refresh();
}

void CompleteCommand::redo()
{
	_task->setComplete(_complete);
	_model->refresh();
}

int CompleteCommand::id() const
/**/
{
	return -1;
}

bool CompleteCommand::mergeWith(const QUndoCommand *other)
/* */
{
	Q_UNUSED(other);
	return false;
}
