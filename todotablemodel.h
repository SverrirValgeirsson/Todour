#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMouseEvent>
#include "todo_backend.h"
#include "task.h"
#include <QUndoStack>
#include <vector>



#define TODOUR_INACTIVE "TODOUR_INACTIVE_794e26fdf5ea"


class TodoTableModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
//    todotxt *todo; // interface with files
  todo_backend  *todo;
    vector<task*> task_set;

public:
    explicit TodoTableModel(QUndoStack* undo, QObject *parent = 0);
    ~TodoTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
 	bool setData(const QModelIndex & index, const QVariant & value, int role);
    int count();

    void addTask(task* t);
    task* removeTask(QUuid tuid);
	task* getTask(QUuid tuid);
	task* getTask(QModelIndex index);

    void archive();
    void refresh();
    int flush();
            
   inline void clearFileWatch(){   todo->clearMonitoring();}; //gaetan 5/1/24
   inline void setFileWatch(QObject *parent){   todo->setMonitoring(parent);}; //gaetan 5/1/24


signals:
	void dataSavedOK();
	
public slots:
    void backendDataLoaded();
    void backendDataSaved();
    void backendError();
private:
	QUndoStack* _undo;
};

#endif // TODOTABLEMODEL_H
