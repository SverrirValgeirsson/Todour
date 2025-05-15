#include "todotxt.h"
#include "def.h"

// Todo.txt file format: https://github.com/ginatrapani/todo.txt-cli/wiki/The-Todo.txt-Format

#include <QTextStream>
#include <QStringList>
#include <set>
#include <QSettings>
#include <QList>
#include <QVariant>
#include <QDebug>
#include <QUuid>
#include <QDir>
#include <vector>

#include <QFileSystemWatcher>

todotxt::todotxt(QObject *parent) :
	QObject(parent)
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
{
//    if(undoDir) delete undoDir;

	delete _TodoFile;
	delete _DoneFile;
	delete _DeleteFile;
	

}

void todotxt::getAllTask(vector<task*> &output)
// Load all tasks from file to "output"  APPEND ???
// Implement an error system, emit FileError
{
    QSettings settings;
    _TodoFile->open(QIODevice::ReadOnly | QIODevice::Text);

    if (!_TodoFile->isOpen()){
    	qDebug()<<"todotxt::getAllTask error opening file"<<endline;
    	return;
    }

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

// GDE-NTM need rework
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

int todotxt::write(vector<task*>& content, filetype t, bool append)
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
		case typetodofile:
		    if (append) result=_TodoFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
			else result =_TodoFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result) return 1;
			out.setDevice(_TodoFile);
		    break;
		case typedonefile:
		    if (append) result=_DoneFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		    else result=_DoneFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result) return 1;
			out.setDevice(_DoneFile);
		    break;
		case typedeletefile:
		    if (append) result=_DeleteFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
		    else result=_DeleteFile->open(QIODevice::WriteOnly | QIODevice::Text);
			if (!result) return 1;
			out.setDevice(_DeleteFile);
		    break;
	default: return 2;
	}


//#TODO change this to iterator.
     out.setEncoding(QStringConverter::Utf8);
     for(unsigned int i = 0; i<content.size(); i++)
         out << content.at(i)->toSaveString() << "\n";

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
     return 0;
}





QFileSystemWatcher *watcher;
void todotxt::clearFileWatch()
{
    if(watcher != NULL){
        delete watcher;
        watcher = NULL;
    }
}

void todotxt::setFileWatch(QObject *parent)
{
	Q_UNUSED(parent);
    watcher = new QFileSystemWatcher();
    watcher->removePaths(watcher->files()); // Make sure this is empty. Should only be this file we're using in this program, and only one instance
    watcher->addPath(_TodoFilePath);
    QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileModified(QString)));

}


void todotxt::fileModified(QString str)
/*
*/
{
	Q_UNUSED(str);
	qDebug()<<"DEBUG : todotxt::fileModified detected !!"<<endline;

/*   COPY FROM MAINWINDOW.CPP:
void MainWindow::fileModified(const QString &str)
  
    Q_UNUSED(str);
    //qDebug()<<"MainWindow::fileModified  "<<watcher->files()<<" --- "<<str;
//    saveTableSelection();  // This should be maintained???
    model->refresh();
    if(model->count()==0){
        // This sometimes happens when the file is being updated. We have gotten the signal a bit soon so the file is still empty. Wait and try again
        delay();
        model->refresh();
        updateTitle();
    }
//    resetTableSelection();  // This should be maintained???
    setFileWatch();
    }
*/    
}
