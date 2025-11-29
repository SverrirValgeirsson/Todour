#ifndef TASKSET_H
#define TASKSET_H
//a taskset represents a set of tasks. Historically, it was grouped with the todotablemodel

#include "todo_backend.h"
#include "task.h"
#include <QUndoStack>
#include <vector>

class todo_backend;

#define TODOUR_INACTIVE "TODOUR_INACTIVE_794e26fdf5ea"
// this should be removed if the model / proxy is good.


class taskset:public QObject
{
	Q_OBJECT
	
private:

protected:
	todo_backend  *todo;
	todo_backend *cdav_client;
   std::vector<task*> content;  //RENAME THIS

public:
    explicit taskset(QUndoStack* undo, QObject *parent = 0);
    ~taskset();

    void addTask(task* t);
    task* removeTask(QUuid tuid);
    
    void safeComplete(int position, bool state);
    void safeEdit(int position, QString _raw);
    void safeAdd(task* _t);
    void safeDelete(QUuid index);
    void safePostpone(int position, QString txt);
    void safePriority(int position, QChar prio);
    
	 task* getTask(QUuid tuid);
	 task* getTask(int position);
	 inline task* at(int position) const {return content.at(position);};
	// inline void push_back(task* _t) {content.push_back(_t);};
	 inline bool empty() const {return content.empty();};
	 inline int size() const {return content.size();};

    
    void refreshActive(); //cycle through all task to recalculate the active state
	int flush();
//NOte: todotablemodel uses  "->size, ->empty, ->at, ->toggleDone
//

    
   void setFileWatch(bool b, QObject *parent);
	QString toString();
	int size();
	void archive();
    void synchronize();
q
signals:
	void dataSavedOK();
	void backendError();
	void sync_error();
	void sync_finished();
	
public slots:
    void backendDataLoaded();
    void backendDataSaved();
    void toggleDone(int tuid);
	void actualSynchronize();
    
private:
	QUndoStack* _undo;
};

#endif //define TASKSET_H
