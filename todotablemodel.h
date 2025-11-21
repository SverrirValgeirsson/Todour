#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMouseEvent>
#include "task.h"
#include <QUndoStack>
#include <vector>
#include "taskset.h"

#define TODOUR_INACTIVE "TODOUR_INACTIVE_794e26fdf5ea"

class TodoTableModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
	taskset* tasklist;
public:
    explicit TodoTableModel(taskset* _tasklist, QObject *parent = 0);
    ~TodoTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
 	 bool setData(const QModelIndex & index, const QVariant & value, int role);
	 inline task* getTask(QModelIndex index) {return (tasklist->at(index.row()));};

    void refresh();    
//    void refreshActive(); //cycle through all task to recalculate the active state
	QString toString();

signals:
	
public slots:
private:

};


class IdeaTableModel : public TodoTableModel
{
    Q_OBJECT
protected:
public:
    explicit IdeaTableModel(taskset* _tasklist, QObject *parent = 0);
    ~IdeaTableModel();
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
 	 bool setData(const QModelIndex & index, const QVariant & value, int role);

//    void refresh();    

signals:
	
public slots:
private:

};


#endif // TODOTABLEMODEL_H
