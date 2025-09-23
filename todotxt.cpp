#include "todotxt.h"
#include "def.h"

// Todo.txt file format: https://github.com/ginatrapani/todo.txt-cli/wiki/The-Todo.txt-Format

#include <QTextStream>
#include <QSettings>
#include <QDebug>

#include <QFileSystemWatcher>

todotxt::todotxt(QObject *parent) :
	todo_backend(parent)
{
    QSettings settings;
    QString dir = settings.value(SETTINGS_DIRECTORY).toString();
	_TodoFile = new QFile(dir + TODOFILE);
	_DoneFile = new QFile(dir + DONEFILE);
	_DeleteFile=new QFile(dir + DELETEDFILE);
		
    _TodoFilePath = dir + TODOFILE;
    _DoneFilePath = dir + DONEFILE;
    _DeleteFilePath = dir + DELETEDFILE;

}

todotxt::~todotxt()
/* */
{

	delete _TodoFile;
	delete _DoneFile;
	delete _DeleteFile;
}

bool todotxt::isReady()
/* if true, can receive readRequest and writeRequest */
{
	return _TodoFile->exists();
}

void todotxt::reloadRequest()
/* */
{
    _TodoFile->open(QIODevice::ReadOnly | QIODevice::Text);

    if (!_TodoFile->isOpen()){
    	qDebug()<<"todotxt::getAllTask error opening file"<<endline;
    	return;
    }
	emit DataAvailable();
}

void todotxt::getAllTask(vector<task*> &output)
/* Load all tasks from file to "output"  APPEND ???
 Implement an error system, emit FileError
*/
{
    QSettings settings;

    QTextStream in(_TodoFile);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(settings.value(SETTINGS_REMOVE_DOUBLETS,DEFAULT_REMOVE_DOUBLETS).toBool()){
            // This can be optimized by for example using a set<QString>
            //if(std::find(content.begin(),content.end(),line) != content.end()){
                // We found this line. So we ignore it
            //    continue;
            //}
        }
        if (!line.isEmpty()){
			task* newt = new task(line,"",true);
			output.push_back(newt);
			}
     }
	if (_TodoFile->isOpen()){
    	_TodoFile->close();	
	}
}

void todotxt::writeRequest(vector<task*>& content, TodoDestination t, bool append)
/* Writes a set of tasks (vector) to the file selected by filetype.
If append is false, overwrite the file.
return 1 : cannot open file
return 2 : wrong "filetype", did nothing
return 0 : sucess.
*/
{
    qDebug()<<"todotxt::writeB("<<t<<"). append="<<append<<endline;
    QTextStream out;
	bool result;
	switch (t){
		case typeTodo:
		    if (append) result=_TodoFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
			else result =_TodoFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result){
				emit WriteError("todotxt Write: unable to open todo file");
				return;
			}
			out.setDevice(_TodoFile);
		    break;
		case typeDone:
		    if (append) result=_DoneFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		    else result=_DoneFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result){
				emit WriteError("todotxt Write: unable to open done file");
				return;
			}
			out.setDevice(_DoneFile);
		    break;
		case typeDelete:
		    if (append) result=_DeleteFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		    else result=_DeleteFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result){
				emit WriteError("todotxt Write: unable to open deleted file");
				return;
			}
			out.setDevice(_DeleteFile);
		    break;
	default: 
		emit WriteError("todotxt Write: unkown file type");
		return;
	}
	
	
    out.setEncoding(QStringConverter::Utf8);
    for(vector<task*>::iterator i=content.begin(); i!=content.end(); i++)
        {
        qDebug()<<"Writing: "<<(*i)->toSaveString()<<endline;
         out << (*i)->toSaveString() << "\n";
         }

	if (_TodoFile->isOpen()){
	     _TodoFile->flush();
   		 _TodoFile->close();
	}
	if (_DoneFile->isOpen()){
     	_DoneFile->flush();
    	_DoneFile->close();	
	}
	if (_DeleteFile->isOpen()){
	     _DeleteFile->flush();
	     _DeleteFile->close();	
	}
    emit DataSaved();
    return;
}

QFileSystemWatcher *watcher;
void todotxt::setMonitoring(bool b, QObject *parent)
/* */
{
	Q_UNUSED(parent);
	if (b) {
		qDebug()<<"Filesystemwtcher activated"<<endline;
    	watcher = new QFileSystemWatcher();
    	watcher->removePaths(watcher->files()); // Make sure this is empty. Should only be this file we're using in this program, and only one instance
    	watcher->addPath(_TodoFilePath);
    	QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SIGNAL(DataChanged()));
		}
	else {
 	   if(watcher != NULL){
        delete watcher;
        watcher = NULL;
	}
}
}

QString todotxt::getType()
/* return type of Backend*/
{
	return "todotxt";
}

// #TODO  delete this?
// re-use the code of "REMOVE_DOUBLETS"
void todotxt::slurp(QFile* file,vector<QString>& content){ //  NOT USED.
qDebug()<<" use of deprecated todotxt::slurp"<<endline;
    QSettings settings;
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(file);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(settings.value(SETTINGS_REMOVE_DOUBLETS,DEFAULT_REMOVE_DOUBLETS).toBool()){
            // This can be optimized by for example using a set<QString>
            if(std::find(content.begin(),content.end(),line) != content.end()){
                // We found this line. So we ignore it
                continue;
            }
        }
        content.push_back(line);
     }
	if (file->isOpen()){
    	file->close();	
	}
}

