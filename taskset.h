#ifndef TASKSET_H
#define TASKSET_H
//a taskset represents a set of tasks. It gives access to the vector with some add work.

#include "task.h"
#include <QUndoCommand>
#include <vector>

class todo_backend;

class taskset:public QObject
{
	Q_OBJECT
	
private:

protected:
	todo_backend  *todo;
   std::vector<task*> content;

public:
   explicit taskset(QObject *parent = 0);
   ~taskset();
	inline task* at(int position) const {return content.at(position);};



//for todoundo
   void addTask(task* t);
   void removeTask(QUuid tuid);
   task* getTask(QUuid tuid);
      

//for mainwindow 
	void flush();
	void archive();
	inline QStringList getContexts(){return contexts;};
	QStringList getFullContexts();
   void setFileWatch(bool b, QObject *parent);


//for tablemodel
	inline int size() const {return content.size();};
	inline bool empty() const {return content.empty();};  //only in todotableModel

//for debug
	QString toString();

signals:
	void dataSavedOK();
	void backendError(QString);
	
public slots:
    void backendDataLoaded();
    void recalculate();
    
private:
	QStringList contexts;
	QStringList full_contexts;
	QStringList inactiveFlags;
};



#include "notetxt.h"

class noteset : public QObject
{

Q_OBJECT

public:
	noteset(QObject* parent);
	~noteset();

	void handleTextChanged(QString newtext); // text changed in UI, write it on disk. Only launched by save or equiv.

   void setFileWatch(bool b, QObject *parent); //cannot be used.
	void flush();
	QString toString();
	void reLoad();	

	
public slots:
   void backendDataLoaded();						// text-file rady to load

signals:
	void backendError(QString txt);
	void dataSavedOK();
	void updateText(QString txt);

private:
	notetxt* notes;
	QString content;
};


#endif //define TASKSET_H
