#include "todotablemodel.h"
#include "def.h"

#include <QFont>
#include <QSettings>
#include <QRegularExpression>
#include <QDebug>
#include <vector>

#include "todo_undo.h"


TodoTableModel::TodoTableModel(QUndoStack* undo, QObject *parent) :
    QAbstractTableModel(parent), _undo(undo)
{
    todo = new todotxt();
	todo->getAllTask(task_set);
	}

TodoTableModel::~TodoTableModel()
{
    delete todo;
}

int TodoTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
//Should we consider here the quantity of SHOWN rows?
    int size = (int)task_set.size();
    return size;
    }

int TodoTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant TodoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);
    if(role == Qt::DisplayRole)
    {
      if(section==0) return "Done";
      if(section==1) return "Todo";
    }

  return QVariant::Invalid;
}


Qt::ItemFlags TodoTableModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags returnFlags = QAbstractTableModel::flags(index);
  if (index.column() == 0)  {
    returnFlags |= Qt::ItemIsUserCheckable;
  }
  if (index.column() == 1)  {
     returnFlags |= Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
  }
  return returnFlags;
}

int TodoTableModel::count(){
    return this->rowCount(QModelIndex());
}

QVariant TodoTableModel::data(const QModelIndex &index, int role) const
/*
*/
{
	QSettings settings;
	if (!index.isValid()) return QVariant();
	if (task_set.empty()) return QVariant();
    if (index.row() >= (int) task_set.size() || index.row() < 0) return QVariant();
    if (role == Qt::DisplayRole || role==Qt::ToolTipRole)
    {
        if(index.column()==1)
        {
        //Here we need to choose between the 2 options: active_only or all.
        // all is easy, just return all the tasks
            QString s=task_set.at(index.row())->getDisplayText();
            return s;
        }
     }

    if (role == Qt::EditRole) {
        if(index.column()==1){
            QString s=task_set.at(index.row())->getEditText();   //ATTENTION : il faut ici afficher plus (color,...) mais pas tout!
//			if (s.length()>30){
//				QItemEditorFactory::registerEditor(QVariant::String,QItemEditorFactory::createEditor(QVariant::String,this->parent()->ui));
//			}
            return s;
        }
     }

    if(role == Qt::CheckStateRole) {
        if(index.column()==0) return task_set.at(index.row())->isComplete();
    }

    if(role == Qt::FontRole) {
        if(index.column()==1){
            QFont f;
            if (! task_set.at(index.row())->isActive()){
            	 f.fromString(settings.value(SETTINGS_INACTIVE_FONT).toString());
            } else {
                 f.fromString(settings.value(SETTINGS_ACTIVE_FONT).toString());
            }
			 f.setStrikeOut(task_set.at(index.row())->isComplete()); // Strike out if done

//            QString url =todo->getURL(todo_data.at(index.row())) ;
//            if(!url.isEmpty()){
//                f.setUnderline(true);
//            }
            return f;
        }
    }

    if (role == Qt::ForegroundRole) 
    {
        int due=INT_MAX;
    	QSettings settings;
    	if(settings.value(SETTINGS_DUE).toBool()){
    		if (task_set.at(index.row())->getDueDate().isValid()){
				due = -(int) (task_set.at(index.row())->getDueDate().daysTo(QDate::currentDate()));
			}
		}
        bool active = true;
        if(task_set.at(index.row())->isComplete()){
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
       			if(!task_set.at(index.row())->isActive()){
            		return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_INACTIVE_COLOR,DEFAULT_INACTIVE_COLOR).toUInt()));
        		}
        		else {
            		return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_ACTIVE_COLOR,DEFAULT_ACTIVE_COLOR).toUInt()));
        		}
    		}

    if (role == Qt::BackgroundRole && index.column()==1){
		if (task_set.at(index.row())->getColor().isValid())
			return QVariant::fromValue(task_set.at(index.row())->getColor().lighter(180));
	}

		//Qt::UserRole is used for sorting. To use the power of regexp and QProxyModel, we will add a special code TODOUR_INACTIVE
    if(role == Qt::UserRole){
		QString _prepend="";
		if (settings.value(SETTINGS_SEPARATE_INACTIVES,DEFAULT_SEPARATE_INACTIVES).toBool())
			_prepend=(QChar) QChar::LastValidCodePoint;
	   
	   	//qDebug()<<"todotablemodel::getData() :"<<_prepend + task_set.at(index.row())->getEditText()+" "+TODOUR_INACTIVE<<endline;
	   	if (task_set.at(index.row())->isActive()){
//	   		qDebug()<<"todotablemodel::getData() :"<<task_set.at(index.row())->getEditText()+" "+TODOUR_INACTIVE<<endline;
	    	return task_set.at(index.row())->getEditText()+" "+TODOUR_INACTIVE;
	    }
	    else{
//	   		qDebug()<<"todotablemodel::getData() :"<<_prepend + task_set.at(index.row())->getEditText();
	    	return _prepend + task_set.at(index.row())->getEditText();
	    }
	 }

	if(role == Qt::UserRole+1)  //UserRole+1 is used to sort by inputdate.
	{
		return task_set.at(index.row())->getInputDate().toString("yyyy-MM-dd");
	}

	return QVariant();
}


bool TodoTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
/* */{
	QAbstractItemModel::beginResetModel();

    if(index.column()==0 && role == Qt::CheckStateRole)
	    	_undo->push(new CompleteCommand(this, task_set.at(index.row()), value.toBool()));
    
    else if(index.column()==1 && role == Qt::EditRole)
			_undo->push(new EditCommand(this, task_set.at(index.row()),value.toString()));
    else return false;
    QAbstractItemModel::endResetModel();
  	return true;
}


void TodoTableModel::addTask(task *_t)
/* */
{
 	if (_t!=0)
	 	    task_set.push_back(_t);
}

task* TodoTableModel::removeTask(QUuid tuid)
/*
Remove the task, don't delete object*/
{

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

task* TodoTableModel::getTask(QUuid tuid)
/* */
{
	for (vector<task*>::iterator i=task_set.begin();i!=task_set.end();++i){
		if ((*i)->getTuid() == tuid){
			return *i;
		}
	}
	return 0;
}

task* TodoTableModel::getTask(QModelIndex index)
/* */
{
	return (task_set.at(index.row()));
}


void TodoTableModel::archive()
/* Remove all the "finished" tasks and move them to the "done" file.
#TODO  use UndoCommands ???
*/
{
	QAbstractItemModel::beginResetModel();

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

    todo->write(task_set,typetodofile,false); // append=false
    todo->write(done_set,typedonefile,true); // append=true
    QAbstractItemModel::endResetModel();
}

int TodoTableModel::flush()
/*  */
{
    return todo->write(task_set,typetodofile,false); // append=false

}

// what is exactly the scope of this?
// it is activated by the click on "Reresh" button, requesting to reload the file from disk.
// This is only possible if we trust more the file than our internal info.
// Options : remove, de-activate
//		or make a check of the file, if different, propose to reload
//		or consider that we are saving any change immediately, and the file is trusted.
void TodoTableModel::refresh()
{
// For the moment, do nothing

//    QAbstractItemModel::beginResetModel();
//    todo->refresh();
//    todo_data.clear();

    QAbstractItemModel::beginResetModel();
    QAbstractItemModel::endResetModel();


    emit dataChanged(QModelIndex(),QModelIndex());

//    QAbstractItemModel::endResetModel();
}
