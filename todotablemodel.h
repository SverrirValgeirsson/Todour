#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include "todotxt.h"

class TodoTableModel : public QAbstractTableModel
{
    Q_OBJECT
protected:
    todotxt *todo;

public:
    explicit TodoTableModel(QObject *parent = 0);
    ~TodoTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    void add(QString text);
    void remove(QString text);
    void archive();
    void refresh();
    int count();
    QString getTodoFile();
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap )) const;
    bool undo();
    bool redo();
    bool undoPossible(); // Say if undo is possible or not
    bool redoPossible(); // Say if redo is possible or not

signals:
   //void dataChanged(QModelIndex i1,QModelIndex i2,QVector<int> v); Borde inte behövas. Det finns ju redan
    
public slots:
    
};

#endif // TODOTABLEMODEL_H
