#include "taskset.h"
#include "def.h"
#include "todotxt.h"
//#include "caldav.h"

#include <QMessageBox>
#include <QRegularExpression>

taskset::taskset(QObject *parent)
/* 
*/{
	Q_UNUSED(parent);

   todo = new todotxt();
	QObject::connect(todo,SIGNAL(DataChanged()),this,SLOT(backendDataLoaded()));//REM
	QObject::connect(todo,SIGNAL(DataAvailable()),this,SLOT(backendDataLoaded()));//REM
	QObject::connect(todo,SIGNAL(DataSaved()),this,SIGNAL(dataSavedOK()));//REM
	if (todo->isReady()) todo->reloadRequest();

}
    
taskset::~taskset()
/* */{
	delete todo;//REM
}


void taskset::addTask(task* _t)
/* 
*/{
  	if (_t!=nullptr) content.push_back(_t);
  	this->recalculate();
}

task* taskset::getTask(QUuid tuid)
/*
*/{
	task* ret = nullptr;
	for (vector<task*>::iterator i=content.begin();i!=content.end();++i){
		if ((*i)->getTuid() == tuid){
			ret= *i;
			return ret;
		}
	}
	return nullptr;

}

void taskset::removeTask(QUuid tuid)
/* Remove the task, don't delete object
*/{
	for (vector<task*>::iterator i=content.begin();i!=content.end();++i){
		if ((*i)->getTuid() == tuid){
			content.erase(i);
			return;
		}
	}
}

void taskset::flush()
/*  backend will emit signals, connect done above.
*/{
   todo->writeRequest(content,typeTodo,false); // append=false
}

QStringList taskset::getFullContexts()
/*
*/{
	QStringList ret = contexts + full_contexts;
	ret.removeDuplicates();
	return ret;
	}


QString taskset::toString()
/* 
*/{
	return QString("taskset: tasks contained: ") + QString::number(size());
}
    
void taskset::backendDataLoaded()
/* Called when the backend is ready to supply data.
*/{
  	todo->getAllTask(content);  
	recalculate();
}

void taskset::archive()
/* Remove all the "finished" tasks and move them to the "done" file.
*/{
    vector<task*> done_set;
    for(vector<task*>::const_iterator iter=content.begin();iter!=content.end();){
        if((*iter)->isComplete()){
            done_set.push_back((*iter));
            iter=content.erase(iter);
        }
        else // No change
        		iter++;
     }
	this->flush();
    todo->writeRequest(done_set,typeDone,true); // append=true
}


void taskset::setFileWatch(bool b, QObject *parent)
/* */{
   todo->setMonitoring(b, parent);
}

void taskset::recalculate()
/* Data of display has been modified, refresh the internal data:
- through all tasks, 
*/{
	QSettings settings;
	contexts.clear();
	full_contexts.clear();
	inactiveFlags = settings.value(SETTINGS_INACTIVE,DEFAULT_INACTIVE).toString().split(";");
	for (vector<task*>::iterator itask=content.begin();itask!=content.end();++itask){
		for(QString s:(*itask)->getContexts()){
			if (! contexts.contains(s)) contexts<<s;			
			}
		for(QString s:(*itask)->getThresholdContexts()){
			if (! full_contexts.contains(s)) full_contexts<<s;
			}

			//check all tasks for inactive keywords
		(*itask)->recalculateTask(inactiveFlags);
	}
}


//*************************************************************************************************************************

noteset::noteset(QObject* parent)
/*
*/{
	Q_UNUSED(parent)
	notes=new notetxt(this);
	content="";	
	connect(notes,SIGNAL(WriteError(QString)),this, SIGNAL(backendError(QString)));
	connect(notes,SIGNAL(ReadError(QString)),this, SIGNAL(backendError(QString)));
	connect(notes,SIGNAL(DataSaved()),this, SIGNAL(dataSavedOK()));
	connect(notes,SIGNAL(DataAvailable()),this, SLOT(backendDataLoaded()));
	connect(notes,SIGNAL(DataChanged()),this, SLOT(backendDataLoaded()));
}

void noteset::reLoad()
/*
*/{
	notes->reloadRequest();
}


noteset::~noteset()
/*
*/{
	delete notes;
}

void noteset::setFileWatch(bool b, QObject *parent)
/*
*/{
	Q_UNUSED(parent)
	Q_UNUSED(b)
	notes->setMonitoring(false,this); //disable filewatcher because we keep our version in memory and not on file...
}

QString noteset::toString()
/*
*/{
	return notes->toString();
}

void noteset::handleTextChanged(QString newtext)
/* receive text from UI, save it.
*/{

	notes->writeRequest(newtext);
}

void noteset::backendDataLoaded()
/*
*/{
	notes->getAllText(&content);
	emit updateText(content);
}


void noteset::flush()
/*
*/{
// Nothing to do.
}


