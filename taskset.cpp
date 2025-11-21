#include "taskset.h"
#include "def.h"
#include "todo_undo.h"


#include "todotxt.h"
#include "caldav.h"

taskset::taskset(QUndoStack* undo, QObject *parent):
	_undo(undo)
/* */{
    Q_UNUSED(parent);
     	QSettings settings;

    todo = new todotxt();

    // 	if (settings.value(SETTINGS_BACKEND).toString()=="caldav"){
// 		todo = new caldav();
// 		}

	QObject::connect(todo,SIGNAL(DataChanged()),this,SLOT(backendDataLoaded()));//REM
	QObject::connect(todo,SIGNAL(ConnectionLost()),this,SIGNAL(backendError()));//REM
	QObject::connect(todo,SIGNAL(DataAvailable()),this,SLOT(backendDataLoaded()));//REM
	QObject::connect(todo,SIGNAL(DataSaved()),this,SLOT(backendDataSaved()));//REM
	if (todo->isReady())//REM
		todo->reloadRequest();//REM


qDebug()<<"SCOPE09: taskset created and connect done"<<endline;
    }
    
taskset::~taskset()
/* */{
        delete todo;//REM
}
    
void taskset::addTask(task* _t)
/* */{
  	if (_t!=nullptr) task_set.push_back(_t);    
}


////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Safe commands generate the action through the _undo stack


void taskset::safeComplete(int position, bool state)
/* Safely complete the tasks at position, creating an undo command
*/{
	_undo->push(new CompleteCommand(this, task_set.at(position), state));

}
   
void taskset::safeEdit(int position, QString _raw)
/* Safely edit the task at position, creating an undo command
*/{
	_undo->push(new EditCommand(this, task_set.at(position),_raw));
}

void taskset::safeAdd(task* t)
/* Safely add a task, creating an undo command
*/{
    QSettings settings;
    QString prio="";
  	 _undo->push(new AddCommand(this,t));
}

void taskset::safeDelete(QUuid index)
/* Safely delete a task, creating an undo command
*/{
	_undo->push(new DeleteCommand(this,index));
}

void taskset::safePostpone(int position, QString txt)
/* Safely postpone a task, creating an undo command
*/{
	_undo->push(new PostponeCommand(this, task_set.at(position), txt));
}

void taskset::safePriority(int position, QChar prio)
/* Safely change priority of a task, creating an undo command
*/ {
	_undo->push(new PriorityCommand(this,task_set.at(position), prio));
}


////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Other commands generate the action directly

task* taskset::removeTask(QUuid tuid)
/* Remove the task, don't delete object*/{
	Q_UNUSED(tuid);
	//	qDebug()<<"TodoTableModel::removeTask "<<tuid<<endline;
	task* ret = nullptr;
	for (vector<task*>::iterator i=task_set.begin();i!=task_set.end();++i){
		if ((*i)->getTuid() == tuid){
//			qDebug()<<"   found task: "<<(*i)->toString()<<endline;
			ret= *i;
			task_set.erase(i);
			return ret;
		}
	}
	return nullptr;
}
    

task* taskset::getTask(QUuid tuid)
/* */{
	for (vector<task*>::iterator i=task_set.begin();i!=task_set.end();++i){
		if ((*i)->getTuid() == tuid)return *i;
	}
	return 0;    
}
    
task* taskset::getTask(int position)
/* */{
	Q_UNUSED(position);
    return nullptr;
}
    
//    void archive();
//    void refresh();
int taskset::flush()
/*  
*/{
   todo->writeRequest(task_set,typeTodo,false); // append=false
	return 0;    //#TODO change this: we receive a signal when write is ok.
}
    
void taskset::refreshActive()
/* */{
    for (int i=0;i<task_set.size();i++){
    	task_set.at(i)->refreshActive(QDateTime::currentDateTime());
    	}
}
    
    
int taskset::size()
/* 
*/{
    return (int)task_set.size();;
}
    
//cycle through all task to recalculate the active state
    
//   inline void clearFileWatch(){   todo->clearMonitoring();}; //gaetan 5/1/24



QString taskset::toString()
/* */{
	return "";    
}
    
void taskset::backendDataLoaded()
/* */{
  	todo->getAllTask(task_set);  
  		qDebug()<<"void taskset::backendDataLoaded() - all tasks loaded"<<endline;

}
    
void taskset::backendDataSaved()
/* 
*/{
	emit dataSavedOK();
}
    

void taskset::toggleDone(int position)
    /* */{
	//TODO: URGENT: update the undo framework for work with taskset and not model.
//	_undo->push(new CompleteCommand(this, task_set.at(position)));   
    }


void taskset::archive()
/* Remove all the "finished" tasks and move them to the "done" file.
#TODO  use UndoCommands ???
*/
{
//	QAbstractItemModel::beginResetModel();

    vector<task*> done_set;
	qDebug()<<"TodoTableModel::archive task_set.size="<<task_set.size()<<endline;
    for(vector<task*>::const_iterator iter=task_set.begin();iter!=task_set.end();){
        if((*iter)->isComplete()){
        	qDebug()<<"TodoTableModel::archive move task "<<(*iter)->getEditText()<<endline;
            done_set.push_back((*iter));
            iter=task_set.erase(iter);
        } else {
            // No change
            iter++;
        }
     }

    todo->writeRequest(task_set,typeTodo,false); // append=false
    todo->writeRequest(done_set,typeDone,true); // append=true
//    QAbstractItemModel::endResetModel();
}


void taskset::setFileWatch(bool b, QObject *parent)
/* */{
   todo->setMonitoring(b, parent);
}
