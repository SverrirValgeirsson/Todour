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
    
////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Safe commands generate the action through the _undo stack


void taskset::safeComplete(int position, bool state)
/* Safely complete the tasks at position, creating an undo command
*/{
	_undo->push(new CompleteCommand(this, content.at(position), state));

}
   
void taskset::safeEdit(int position, QString _raw)
/* Safely edit the task at position, creating an undo command
*/{
	_undo->push(new EditCommand(this, content.at(position),_raw));
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
	_undo->push(new PostponeCommand(this, content.at(position), txt));
}

void taskset::safePriority(int position, QChar prio)
/* Safely change priority of a task, creating an undo command
*/ {
	_undo->push(new PriorityCommand(this,content.at(position), prio));
}


////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Other commands generate the action directly

void taskset::addTask(task* _t)
/* 
*/{
  	if (_t!=nullptr) content.push_back(_t);    
}

task* taskset::removeTask(QUuid tuid)
/* Remove the task, don't delete object
*/{
	Q_UNUSED(tuid);
	//	qDebug()<<"TodoTableModel::removeTask "<<tuid<<endline;
	task* ret = nullptr;
	for (vector<task*>::iterator i=content.begin();i!=content.end();++i){
		if ((*i)->getTuid() == tuid){
//			qDebug()<<"   found task: "<<(*i)->toString()<<endline;
			ret= *i;
			content.erase(i);
			return ret;
		}
	}
	return nullptr;
}
    

task* taskset::getTask(QUuid tuid)
/* 
*/{
	for (vector<task*>::iterator i=content.begin();i!=content.end();++i){
		if ((*i)->getTuid() == tuid)return *i;
	}
	return 0;    
}
    
task* taskset::getTask(int position)
/* 
*/{
	Q_UNUSED(position);
    return nullptr;
}

int taskset::flush()
/*  
*/{
   todo->writeRequest(content,typeTodo,false); // append=false
	return 0;    //#TODO change this: we receive a signal when write is ok.
}
    
void taskset::refreshActive()
/* 
*/{
    for (unsigned int i=0;i<content.size();i++){
    	content.at(i)->refreshActive(QDateTime::currentDateTime());
    	}
}
  
int taskset::size()
/* 
*/{
    return (int)content.size();;
}
    
//cycle through all task to recalculate the active state
    
//   inline void clearFileWatch(){   todo->clearMonitoring();}; //gaetan 5/1/24



QString taskset::toString()
/* 
*/{
	return "";    
}
    
void taskset::backendDataLoaded()
/* 
*/{
  	todo->getAllTask(content);  
  		qDebug()<<"void taskset::backendDataLoaded() - all tasks loaded"<<endline;
}
    
void taskset::backendDataSaved()
/* 
*/{
	emit dataSavedOK();
}
    

void taskset::toggleDone(int position)
/* 
*/{
Q_UNUSED(position);
	//TODO: URGENT: update the undo framework for work with taskset and not model.
//	_undo->push(new CompleteCommand(this, content.at(position)));   
    }


void taskset::archive()
/* Remove all the "finished" tasks and move them to the "done" file.
#TODO  use UndoCommands ???
*/{
//	QAbstractItemModel::beginResetModel();

    vector<task*> done_set;
	qDebug()<<"TodoTableModel::archive content.size="<<content.size()<<endline;
    for(vector<task*>::const_iterator iter=content.begin();iter!=content.end();){
        if((*iter)->isComplete()){
        	qDebug()<<"TodoTableModel::archive move task "<<(*iter)->getEditText()<<endline;
            done_set.push_back((*iter));
            iter=content.erase(iter);
        } else {
            // No change
            iter++;
        }
     }

    todo->writeRequest(content,typeTodo,false); // append=false
    todo->writeRequest(done_set,typeDone,true); // append=true
//    QAbstractItemModel::endResetModel();
}


void taskset::setFileWatch(bool b, QObject *parent)
/* */{
   todo->setMonitoring(b, parent);
}

//SIGNAL	void sync_error();
//SIGNAL void sync_finished();
	    
void taskset::synchronize()
/* start a sync.   
*/{
	//1. get data from caldav
	cdav_client = new caldav();
	QObject::connect(cdav_client,SIGNAL(DataAvailable()),this,SLOT(actualSynchronize()));//REM
	cdav_client->reloadRequest();
	qDebug()<<"taskset::synchronize() started"<<endline;
}

void taskset::actualSynchronize()
/*  do the sync    3. emit sync_error() or sync_finished()
*/{
		std::vector<task*> cdav;
		cdav_client->getAllTask(content);  
  		qDebug()<<"void taskset::actualSynchronize all tasks loaded from cdav"<<endline;
	//2.check every items
	bool found;
	for (unsigned int i=0;i<content.size();i++){  //Pour chaque item de L1
		found=false;// Found=false
		for (unsigned int j=0;i<cdav.size();i++){//  Pour chaque item de L2
			if(*content[i]==*cdav[j]){// Si L1.id = L2.id
				found=true;  //Found=true
				if (content[i]->getTimeStamp() == cdav[j]->getTimeStamp())//Si L1.date == L2.date    break
					break;
				if (content[i]->getTimeStamp() < cdav[j]->getTimeStamp()){   //Si L1.date < L2.date    copy from L1 to L2; break
					qDebug()<<"taskset::synchronize() copy from todotxt to cdav"<<endline;
					break;
					}
				if (content[i]->getTimeStamp() > cdav[j]->getTimeStamp()){ //So L1.date > L2.date   copy from L2 to L1;break
					qDebug()<<"taskset::synchronize() copy from cdav to todotxt"<<endline;
					break;
					}
				}
			}
		if (!found){ // If (! Found)     copy from L1 to L2 ;
			qDebug()<<"taskset::synchronize() not found, copy from todotxt to cdav"<<endline;
		}
	}
   for (unsigned int i=0;i<cdav.size();i++){  //Pour chaque item de L2
		found=false;// Found=false
		for (unsigned int j=0;i<content.size();i++){//  Pour chaque item de L1
			if(*cdav[i]==*content[j]){// Si L1.id = L2.id
				found=true;  //Found=true
				break;
				}
			}
		}
	if (!found){ // If (! Found)     copy from L1 to L2 ;
		qDebug()<<"taskset::synchronize() not found, copy from cdav to todotxt"<<endline;
	}

emit sync_finished();
}

