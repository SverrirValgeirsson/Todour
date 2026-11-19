#include "todotablemodel.h"
#include "def.h"

#include <QFont>
#include <QRegularExpression>
#include <QDebug>
#include <vector>

#include "todo_undo.h"

TodoTableModel::TodoTableModel(taskset* _tasklist, QUndoStack* _undo, QObject *parent) :
    QAbstractTableModel(parent), tasklist(_tasklist), undoS(_undo)
{}

TodoTableModel::~TodoTableModel(){}

int TodoTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
//Should we consider here the quantity of SHOWN rows?
    int size = (int)tasklist->size();
    return size;
    }

int TodoTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
 }


QVariant TodoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
/*
*/{
    Q_UNUSED(orientation);
    if(role == Qt::DisplayRole)
    {
      if(section==0) return "Done";
      if(section==1) return "Todo";
    }
  return QVariant::Invalid;
}


Qt::ItemFlags TodoTableModel::flags(const QModelIndex& index) const
/*
*/{
  Qt::ItemFlags returnFlags = QAbstractTableModel::flags(index);
  if (index.column() == 0)  {
    returnFlags |= Qt::ItemIsUserCheckable;
  }
  if (index.column() == 1)  {
     returnFlags |= Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
  }
  return returnFlags;
}


QVariant TodoTableModel::data(const QModelIndex &index, int role) const
/* returns data requested by the modelView
*/{
	QSettings settings;
	if (!index.isValid()) return QVariant();
	if (tasklist->empty()) return QVariant();
    if (index.row() >= (int) tasklist->size() || index.row() < 0) return QVariant();
    if (role == Qt::DisplayRole || role==Qt::ToolTipRole){
        if(index.column()==1){
            return QString(tasklist->at(index.row())->getDisplayText());
        }
     }

    if (role == Qt::EditRole) {
        if(index.column()==1){
            QString s=tasklist->at(index.row())->getEditText();
            return s;
        }
     }

    if(role == Qt::CheckStateRole) {
        if(index.column()==0) return tasklist->at(index.row())->isComplete();
    }

    if(role == Qt::FontRole) {
        if(index.column()==1){
            QFont f;
            if (! tasklist->at(index.row())->isActive()){
            	 f.fromString(settings.value(SETTINGS_INACTIVE_FONT).toString());
            } else {
                 f.fromString(settings.value(SETTINGS_ACTIVE_FONT).toString());
            }
			 f.setStrikeOut(tasklist->at(index.row())->isComplete());

            return f;
        }
    }

    if (role == Qt::ForegroundRole) {
        int due=INT_MAX;
    	QSettings settings;
    	if(settings.value(SETTINGS_DUE).toBool()){
    		if (tasklist->at(index.row())->getDueDate().isValid()){  //CHANGE
				due = -(int) (tasklist->at(index.row())->getDueDate().daysTo(QDateTime::currentDateTime()));  //CHANGE
			}
		}
        bool active = true;
        if(tasklist->at(index.row())->isComplete()){ //CHANGE
            active = false;
        }

        if(active && due<=0){
            // We have passed due date
        	return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_DUE_LATE_COLOR,DEFAULT_DUE_LATE_COLOR).toUInt()));
        }
        else 
        	if(active && due<=settings.value(SETTINGS_DUE_WARNING,DEFAULT_DUE_WARNING).toInt()){
            	return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_DUE_WARNING_COLOR,DEFAULT_DUE_WARNING_COLOR).toUInt()));
        	}
       		else 
       			if(!tasklist->at(index.row())->isActive()){ //CHANGE
            		return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_INACTIVE_COLOR,DEFAULT_INACTIVE_COLOR).toUInt()));
        		}
        		else {
            		return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_ACTIVE_COLOR,DEFAULT_ACTIVE_COLOR).toUInt()));
        		}
    		}

    if (role == Qt::BackgroundRole && index.column()==1){ // proxy: soit rien ne change, soit on désactive les couleurs de fond en idea?
		if (tasklist->at(index.row())->getColor()->isValid()) //CHANGE
				return QVariant::fromValue(tasklist->at(index.row())->getColor()->lighter(180));  //CHANGE
	}

	if(role == Qt::UserRole+1)  //UserRole+1 returns inputdate
			return QVariant(tasklist->at(index.row())->getInputDate());

	if(role == Qt::UserRole+2)  //UserRole+2 returns active state (bool)
			return tasklist->at(index.row())->isActive();

	if(role == Qt::UserRole+3)  //UserRole+3 returns thresholddate
			return QVariant(tasklist->at(index.row())->getThresholdDate());

	if(role == Qt::UserRole+4)  //UserRole+4 returns duedate
			return QVariant(tasklist->at(index.row())->getDueDate());

	if(role == Qt::UserRole+5)  //UserRole+5 returns thresholdContexts
			return QVariant(tasklist->at(index.row())->getThresholdContexts());
	
	if(role == Qt::UserRole+6)  //UserRole+6 returns Priority
			return QVariant(tasklist->at(index.row())->getPriority());

	if(role == Qt::UserRole+7)  //UserRole+6 returns Checked
			return QVariant(tasklist->at(index.row())->isComplete());
			
	if(role == Qt::UserRole+8)  //UserRole+8 returns Contexts
		return QVariant(tasklist->at(index.row())->getContexts());
	

   
	return QVariant();
}


bool TodoTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
/* 
*/{
   if(index.column()==0 && role == Qt::CheckStateRole)
  	 		this->safeComplete(index,value.toBool());  
   else{ if(index.column()==1 && role == Qt::EditRole)
			{this->safeEdit(index,value.toString());}
   	else return false;
   	}

	tasklist->recalculate();
   return true;
}

void TodoTableModel::dataChanged(QModelIndex index)
/*
*/{
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
}



////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Safe commands generate the action through the _undo stack

void TodoTableModel::safeComplete(const QModelIndex & index, bool state)
/* Safely complete the tasks at position, creating an undo command
*/{
	undoS->push(new CompleteCommand(tasklist, tasklist->at(index.row()), state));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
	}
	
void TodoTableModel::safeEdit(const QModelIndex & index, QString _raw)
/* Safely edit the task at position, creating an undo command
*/{
	undoS->push(new EditCommand(tasklist, tasklist->at(index.row()),_raw));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
	}
	
void TodoTableModel::safeAdd(task* _t)
/* Safely add a task, creating an undo command
*/{
	undoS->push(new AddCommand(tasklist,_t));
  	}

void TodoTableModel::safeAdd(QString s, QString c)
/* Safely add a task, creating an undo command
*/{
	qDebug()<<"TodoTableModel::safeAdd"<<endline;
	undoS->push(new AddCommand(tasklist,s, c));
  	}

void TodoTableModel::safeDelete(QModelIndexList indexes)
/* Safely delete tasks, creating undo commands
*/{
    QList<QUuid> tuidL;
	if (!indexes.isEmpty()){
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
			    tuidL.push_back(tasklist->at(i->row())->getTuid());
	
		startModelChange();
		undoS->beginMacro("deletion"); //do not change this text!
		for (QList<QUuid>::iterator j=tuidL.begin();j!=tuidL.end();++j)
				undoS->push(new DeleteCommand(tasklist,tasklist->getTask(*j)));
		undoS->endMacro(); 
		endModelChange();		

	}
}


void TodoTableModel::safePostpone(const QModelIndex & index, QString txt)
/* Safely postpone a task, creating an undo command
*/{
	undoS->push(new PostponeCommand(tasklist, tasklist->at(index.row()), txt));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
}

void TodoTableModel::safePriority(const QModelIndex & index, QChar prio)
/* Safely change priority of a task, creating an undo command
*/{ 
	undoS->push(new PriorityCommand(tasklist,tasklist->at(index.row()), prio));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
	}
	
void TodoTableModel::safeToggleComplete(const QModelIndex & index)
/* Safely toggle the complete state
*/{
	undoS->push(new CompleteCommand(tasklist, tasklist->at(index.row()), !tasklist->at(index.row())->isComplete()));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
	}
	
void TodoTableModel::safeProgress(const QModelIndex & index)
/* Safely add 20% progress
*/{
	undoS->push(new ProgressCommand(tasklist->at(index.row()), 20));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
}

void TodoTableModel::safeDueDate(const QModelIndex & index, QDateTime d)
/* safely set dueDate to d
*/{
	undoS->push(new DueDateCommand(tasklist->at(index.row()), d));
	emit QAbstractItemModel::dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(1));
}


void TodoTableModel::refresh()
/*// what is exactly the scope of this?
// it is activated by the click on "Reresh" button, requesting to reload the file from disk.
// This is only possible if we trust more the file than our internal info.
// Options : remove, de-activate
//		or make a check of the file, if different, propose to reload
//		or consider that we are saving any change immediately, and the file is trusted.*/
{
//#TODO  if we saved the last modifications, reload from the source.
// if we did not save, shows a confirmation window, as we risk to loose the last changes
//  note: this issue is only with todo.txt files. With a synchronisation-capable, we should be safe to reload.
// For the moment, do nothing

//    QAbstractItemModel::beginResetModel();
//    todo->refresh();
//    todo_data.clear();

//    QAbstractItemModel::beginResetModel();
//    QAbstractItemModel::endResetModel();

	tasklist->recalculate();
//   emit dataChanged(createIndex(0, 0),createIndex(999, 1));

//    QAbstractItemModel::endResetModel();
}

QString TodoTableModel::toString()
/* Debugging function only
*/{
	return QString("model : ")+"\n"+QString("  Contains n tasks: ")+QString::number((int)tasklist->size());
}
