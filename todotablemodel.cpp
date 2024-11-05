#include "todotablemodel.h"
#include "def.h"

#include <QFont>
#include <QColor>
#include <QSettings>
#include <QRegularExpression>
#include <QDebug>

TodoTableModel::TodoTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    todo = new todotxt();
   // todo->parse();
	todo->getAllTask(task_set);
}

TodoTableModel::~TodoTableModel()
{
    delete todo;

}

int TodoTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);

    int size = (int)task_set.size();
    return size;
    }

int TodoTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant TodoTableModel::data(const QModelIndex &index, int role) const {
    QSettings settings;

    if (!index.isValid())
             return QVariant();

	if(task_set.empty()){
		return QVariant();
	}

    if (index.row() >= (int) task_set.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role==Qt::ToolTipRole) {
        if(index.column()==1){
            QString s=task_set.at(index.row()).get_display_text();
            return s;
        }

     }

    if (role == Qt::EditRole) {
        if(index.column()==1){
            QString s=task_set.at(index.row()).get_raw();   //ATTENTION : il faut ici afficher plus (color,...) mais pas tout!

            return s;
        }
     }

    if(role == Qt::CheckStateRole) {
        if(index.column()==0)
        	return task_set.at(index.row()).isComplete();
    }

    if(role == Qt::FontRole) {
        if(index.column()==1){
            QFont f;
            if (! task_set.at(index.row()).isActive()){
            	 f.fromString(settings.value(SETTINGS_INACTIVE_FONT).toString());
            } else {
                 f.fromString(settings.value(SETTINGS_ACTIVE_FONT).toString());
            }
			 f.setStrikeOut(task_set.at(index.row()).isComplete()); // Strike out if done

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
    		if (task_set.at(index.row()).get_due_date().isValid()){
				due = -(int) (task_set.at(index.row()).get_due_date().daysTo(QDate::currentDate()));
			}
		}
        bool active = true;
        if(task_set.at(index.row()).isComplete()){
            active = false;
        }

        if(active && due<=0){
            // We have passed due date
            return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_DUE_LATE_COLOR,DEFAULT_DUE_LATE_COLOR).toUInt()));
        } else if(active && due<=settings.value(SETTINGS_DUE_WARNING,DEFAULT_DUE_WARNING).toInt()){
            return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_DUE_WARNING_COLOR,DEFAULT_DUE_WARNING_COLOR).toUInt()));
        }
        else if(!task_set.at(index.row()).isActive()){
            return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_INACTIVE_COLOR,DEFAULT_INACTIVE_COLOR).toUInt()));
        } else {
            return QVariant::fromValue(QColor::fromRgba(settings.value(SETTINGS_ACTIVE_COLOR,DEFAULT_ACTIVE_COLOR).toUInt()));
        }
    }

    if (role == Qt::BackgroundRole && index.column()==1)
    {
		if (task_set.at(index.row()).get_color().isValid())
			return QVariant::fromValue(task_set.at(index.row()).get_color().lighter(150));
	}

    if(role == Qt::UserRole)
		 return task_set.at(index.row()).get_raw();

    if(role == Qt::UserRole+1)
    	 return task_set.at(index.row()).getURL();

    return QVariant();
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



//WORK ONGOING HERE. NEED REWRITE ;-)
bool TodoTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(index.column()==0 && role == Qt::CheckStateRole)
    {
//        beginResetModel();
        task_set.at(index.row()).set_complete(value.toBool());
		todo->write(task_set, typetodofile, false);
    }
    else if(index.column()==1 && role == Qt::EditRole){
//        beginResetModel();
		task_set.at(index.row()).set_raw(value.toString());
		todo->write(task_set, typetodofile, false);
    }
    else {
        // Nothing changed
        return false;
    }

//  endResetModel();

  emit dataChanged(index,index); // Detta innebär ju också att denna item är den som är selected just nu så vi kan lyssna på den signalen

 return true;
}


void TodoTableModel::add(QString text){
    QAbstractItemModel::beginResetModel();
    // manage undo
	task_set.push_back(*new task(text.replace('\n',' ')));
	todo->write(task_set, typetodofile, false);
//       emit dataChanged(index, index);

    QAbstractItemModel::endResetModel();
}

void TodoTableModel::remove(const QModelIndex &index){
	// manage display + undo + internal taskset + file
	QAbstractItemModel::beginResetModel();
    task_set.erase(task_set.begin()+index.row());
    // If we want to use "deleted file", here is the right place.
	todo->write(task_set, typetodofile,false);
    emit dataChanged(index, index);

    QAbstractItemModel::endResetModel();
}

void TodoTableModel::archive(){
//    QAbstractItemModel::beginResetModel();
    vector<task> done_set;

   for(vector<task>::iterator iter=task_set.begin();iter!=task_set.end();){
        if((*iter).isComplete()){
            done_set.push_back((*iter));
            iter=task_set.erase(iter);
        } else {
            // No change
            iter++;
        }
     }

    todo->write(task_set,typetodofile,false);
    todo->write(done_set,typedonefile,true);
//   emit dataChanged(index, index);

//    QAbstractItemModel::endResetModel();
}

void TodoTableModel::refresh()
// what is exactly the scope of this?
// it is activated by the click on "Reresh" button, requesting to reload the file from disk.
// This is only possible if we trust more the file than our internal info.
// Options : remove, de-activate
//		or make a check of the file, if different, propose to reload
//		or consider that we are saving any change immediately, and the file is trusted.
{
// For the moment, do nothing

//    QAbstractItemModel::beginResetModel();
//    todo->refresh();
//    todo_data.clear();
//     emit dataChanged(QModelIndex(),QModelIndex());

//    QAbstractItemModel::endResetModel();
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
//qDebug()<<"TodoTableModel::flags"<<returnFlags<<endline;

  return returnFlags;
}

QModelIndexList TodoTableModel::match(const QModelIndex &start, int role, const QVariant &value, int hits , Qt::MatchFlags flags) const {
qDebug()<<"TodoTableModel::match   ***  DEPRECATED ***"<<endline;
    Q_UNUSED(start);
    Q_UNUSED(hits);
    Q_UNUSED(flags);
    QModelIndexList ret;
    int rows = this->rowCount(QModelIndex()); // Denna tar och laddar modellen också med data
    // Gå igenom alla rader och leta efter en exakt träff
    for(int i=0;i<rows;i++){
        // Skapa ett index
        QModelIndex index= createIndex(i,1);
        if(value.toString() == this->data(index,role)){
            ret.append(index);
        }
    }

    return ret;
}

bool TodoTableModel::undo()
{
    return todo->undo();
}

bool TodoTableModel::redo()
{
    return todo->redo();
}

bool TodoTableModel::undoPossible()
{
    return todo->undoPossible();
}

bool TodoTableModel::redoPossible()
{
    return todo->redoPossible();
}

int TodoTableModel::count(){
    return this->rowCount(QModelIndex());
}

// gaetandc 4/1/24
void TodoTableModel::clearFileWatch()
{
   todo->clearFileWatch();
}

void TodoTableModel::setFileWatch(QObject *parent)
{
   todo->setFileWatch(parent);
}

void TodoTableModel::append(const QModelIndex & index, QString data)
{
//   QAbstractItemModel::beginResetModel();
//   QString str=todo_data.at(index.row());
    QString str=task_set.at(index.row()).get_raw();
    str = str + " " + data;

	task_set.at(index.row()).set_raw(str);
    emit dataChanged(index, index);
//    QAbstractItemModel::endResetModel();

}

void TodoTableModel::setPriority(const QModelIndex & index,QString prio)
// This should make heavy use of CommonTodoModel isCompleted(), setPriority(), ...
// in first instance, lets try to make it by the text.
{
//	QAbstractItemModel::beginResetModel();
	if (! task_set.at(index.row()).isComplete())
	{
		task_set.at(index.row()).set_priority(prio);
	    emit dataChanged(index, index);
		todo->write(task_set,typetodofile,false);

	}
  // QAbstractItemModel::endResetModel();

}

