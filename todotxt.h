/* The Todo.txt master class.
  It handles the todo.txt and parses out information from it as well as writing it back.
  Also done.txt is handled here if needed
  */

#ifndef TODOTXT_H
#define TODOTXT_H

#define TODOFILE "todo.txt"
#define DONEFILE "done.txt"
#define DELETEDFILE "deleted.txt"

#include <vector>
#include <set>
#include <QString>
#include <QDate>
#include <QTemporaryDir>
#include <QFile>
#include <QObject>

#include "task.h"

using namespace std;

typedef enum {typetodofile,typedonefile,typedeletefile} filetype;

class todotxt : public QObject
{
    Q_OBJECT
public:
    explicit todotxt(QObject *parent = 0);
    ~todotxt();

	void getAllTask(vector<task*> &output);
	void clearFileWatch(); //gaetan 5/1/24
	void setFileWatch(QObject *parent); //gaetan 5/1/24
	int write(vector<task*>& content, filetype t, bool append);

protected:
//   QTemporaryDir *undoDir;
    
    QString _TodoFilePath;
    QString _DoneFilePath;
    QString _DeleteFilePath;

	QFile* _TodoFile;
	QFile* _DoneFile;
	QFile* _DeleteFile;

	bool _ready;
// trash
    vector<QString> todo;
    vector<QString> done;
    set<QString> active_projects;
    set<QString> active_contexts;
 
private:
    void slurp(QFile* filename,vector<QString>&  content);
    
    
public slots:
	void fileModified(QString str);

 
public: 

protected:

private:	
};

#endif // TODOTXT_H
