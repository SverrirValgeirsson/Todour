#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMouseEvent>
#include "todotxt.h"
#include "task.h"

#define TODOUR_INACTIVE "TODOUR_INACTIVE_794e26fdf5ea"


class TodoTableModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
    todotxt *todo; // interface with files

//new GDE:    
    vector<task> task_set;

public:
    explicit TodoTableModel(QObject *parent = 0);
    ~TodoTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
 	bool setData(const QModelIndex & index, const QVariant & value, int role);
    int count();

    void addTask(QString text,QString context="",QString priority="");
    void removeTasks(QModelIndexList &index);
    void archive();
    void refresh();
    void postponeTasks(QModelIndexList & index, QString data);
    void setPriorityTasks(QModelIndexList & index,QString prio);

    inline bool undo(){    return todo->undo();};
    inline bool redo(){    return todo->redo();};
    inline bool undoPossible(){    return todo->undoPossible();}; // Say if undo is possible or not
    inline bool redoPossible(){    return todo->redoPossible();}; // Say if redo is possible or not

   inline void clearFileWatch(){   todo->clearFileWatch();}; //gaetan 5/1/24
   inline void setFileWatch(QObject *parent){   todo->setFileWatch(parent);}; //gaetan 5/1/24

signals:
    
public slots:
    
private:

};

#endif // TODOTABLEMODEL_H
