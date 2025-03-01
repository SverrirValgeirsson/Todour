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
#include <deque>

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

    undoDir = new QTemporaryDir();
    if(!undoDir->isValid()){
        qDebug()<<"Could not create undo dir"<<endline;
    } else {
        qDebug()<<"Created undo dir at "<<undoDir->path()<<endline;
    }
}

todotxt::~todotxt()
{
    if(undoDir) delete undoDir;

	delete _TodoFile;
	delete _DoneFile;
	delete _DeleteFile;
	

}

void todotxt::getAllTask(vector<task> &output)
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
		task* newt = new task(line,true);
		output.push_back(*newt);
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

int todotxt::write(vector<task>& content, filetype t, bool append)
{
    qDebug()<<"todotxt::writeB("<<t<<"). append="<<append<<endline;
    saveToUndo();
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

     out.setEncoding(QStringConverter::Utf8);
     for(unsigned int i = 0; i<content.size(); i++)
         out << content.at(i).toString() << "\n";

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

// GDE-NTM need rework
void todotxt::restoreFiles(QString namePrefix){
Q_UNUSED(namePrefix)
/*  #TODO  do not work with files, but reload the content of good file.
    qDebug()<<"Restoring: "<<namePrefix<<endline;
    qDebug()<<"Pointer: "<<undoPointer<<endline;
    // Copy back files from the undo
    QString newtodo = namePrefix+TODOFILE;
    QString newdone = namePrefix+DONEFILE;
    QString newdeleted = namePrefix+DELETEDFILE;

    if(QFile::exists(newtodo)){
        QFile::remove(getTodoFilePath());
        QFile::copy(newtodo,getTodoFilePath());
    }
    if(QFile::exists(newdone)){
        QFile::remove(getDoneFilePath());
        QFile::copy(newdone,getDoneFilePath());
    }
    if(QFile::exists(newdeleted)){
        QFile::remove(getDeletedFilePath());
        QFile::copy(newdeleted,getDeletedFilePath());
    }*/

}

bool todotxt::undo()
{
    // Check if we can
    if(undoPossible()){
		restoreFiles(undoBuffer.back());
		
		redoBuffer.push_back(undoBuffer.back());
		undoBuffer.pop_back();
		
        return true;
    }
    return false;
}

bool todotxt::redo()
{
    // Check if we can
    if(redoPossible()){
        restoreFiles(redoBuffer.back());
		
		undoBuffer.push_back(redoBuffer.back());
		redoBuffer.pop_back();        
        return true;
    }
    return false;
}


bool todotxt::undoPossible()
{
    if((int) undoBuffer.size()>0)
     	   return true;
    
    return false;
}

bool todotxt::redoPossible()
{
    if((int) redoBuffer.size()>0)
        	return true;
    
    return false;
}

// GDE-NTM need rework
QString todotxt::getUndoDir()
{
    if(undoDir->isValid()){
        return undoDir->path()+"/";
    }

    // The below code is backup for undo directory created in the same folder
    // in case there was a problem with getting a temp directory (shouldn't happen.. but)
    QSettings settings;
    QString uuid = settings.value(SETTINGS_UUID,DEFAULT_UUID).toString();
    QString dirbase = settings.value(SETTINGS_DIRECTORY).toString();
    QString dir = dirbase+".todour_undo_"+uuid+"/";
    // Check that the dir exists
    QDir directory = QDir(dir);
    if(!directory.exists()){
        directory.mkdir(dir);
    }
    return dir;
}

// GDE-NTM need rework
QString todotxt::getNewUndoNameDirAndPrefix()
{
    // Make a file path that is made to have _todo.txt or _done.txt appended to it
    // We want the name to be anything that doesn't crash with any other. This can be linear numbering or just an UUID
    // For now it's UUID but a linear numbering (with a solution to multiple clients running with the same undo-directory) would make
    // more sense and be easier to follow for end-users in case they are looking through the files.
    return getUndoDir()+QUuid::createUuid().toString()+"_";
}

// GDE-NTM need rework
void todotxt::cleanupUndoDir()
{
    // Go through the directory and remove everything that is older than 14 days old.
    // Why 14 days? Because. That's why :)
    // (I simply don't think this is interesting to have configurable.. But simple enough to do in case needed)
    QDir directory(getUndoDir());
    QStringList files = directory.entryList(QDir::Files);
    QDateTime expirationTime = QDateTime::currentDateTime();
    expirationTime = expirationTime.addDays(-14);
    qDebug()<<"Checking for undo files to cleanup..."<<endline;
    foreach(QString filename, files) {
        auto finfo = QFileInfo(directory.filePath(filename));
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
        QDateTime created = finfo.birthTime();
#else // QFileInfo::birthTime was introduced in QT 5.10
        QDateTime created = finfo.created();
#endif
        if(expirationTime.daysTo(created)<0 || !created.isValid()){
            //qDebug()<<"We should remove file (but wont now)"<<filename<<endline;
            if(!QFile::remove(directory.filePath(filename))){
                qDebug()<<"Failed to remove: "<<filename<<endline;
            }
        }
    }
}


// Returns true of there is a need for a new undo
// GDE-NTM need rework
bool todotxt::checkNeedForUndo(){
    // check if the todo.txt is any different from the lastUndo file
    vector<QString> todo;
    vector<QString> lastUndo;

    if(undoBuffer.empty()){
        return true;
    }


    QString undofile = undoBuffer.back()+(TODOFILE);
    slurp(_TodoFile,todo);
//    slurp(undofile,lastUndo);
    if(todo.size()!=lastUndo.size()){
        qDebug()<<"Sizes differ: "<<_TodoFilePath<<" vs "<<undofile<<endline;
        return true;
    }

    // We got this far, we have to go trhough the files line by line
    for(int i=0;i<(int) todo.size();i++){
        if(todo[i] != lastUndo[i]){
            qDebug()<<todo[i]<<" != "<<lastUndo[i]<<endline;
            return true;
        }
    }

    // Files are the same. No need to save a new undo.
    return false;
}

// GDE-NTM need rework
void todotxt::saveToUndo()
{
    // This should be called every time we will read todo.txt

	if (undoBuffer.size()>20)
			undoBuffer.pop_front();
		
	if (redoBuffer.size()>0)
			redoBuffer.clear();
	
    QString namePrefix = getNewUndoNameDirAndPrefix();
    QString newtodo = namePrefix+TODOFILE;
    QString newdone = namePrefix+DONEFILE;
    QString newdeleted = namePrefix+DELETEDFILE;

    QFile::copy(_TodoFilePath,newtodo);
    QFile::copy(_DoneFilePath,newdone);
    QFile::copy(_DeleteFilePath,newdeleted);

    undoBuffer.push_back(namePrefix);
    qDebug()<<"Added to undoBuffer: "<<namePrefix<<endline;
    qDebug()<<"Buffer is now: "<<undoBuffer.size()<<endline;
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
