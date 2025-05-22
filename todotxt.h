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
#include <QString>
#include <QDate>
#include <QFile>
#include <QObject>

#include "task.h"
#include "todo_backend.h"

using namespace std;

class todotxt : public todo_backend
{
public:
    explicit todotxt(QObject *parent = 0);
    ~todotxt();

	void getAllTask(vector<task*> &output);
	void clearMonitoring(); //gaetan 5/1/24
	void setMonitoring(QObject *parent); //gaetan 5/1/24
	void reloadRequest();
	void writeRequest(vector<task*>& content, TodoDestination t, bool append);
	bool isReady();
	
protected:    
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
 
private:
    void slurp(QFile* filename,vector<QString>&  content);
    
    
public slots:
//	void fileModified(QString str);
};

#endif // TODOTXT_H
