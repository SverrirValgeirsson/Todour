#include "notetxt.h"
#include "def.h"

#include <QTextStream>
#include <QSettings>
#include <QDebug>



notetxt::notetxt(QObject *parent)
/*
*/{
	Q_UNUSED(parent)
   QSettings settings;
   QString dir = settings.value(SETTINGS_DIRECTORY).toString();
	_NoteFile = new QFile(dir + NOTEFILE);
   _NoteFilePath = dir + NOTEFILE;
   watcher = NULL;
}

notetxt::~notetxt()
/* 
*/{
	if (_NoteFile->isOpen()){
		_NoteFile->flush();
		_NoteFile->close();
		}
	delete _NoteFile;
}

bool notetxt::isReady()
/* if true, can receive readRequest and writeRequest 
*/{
	return _NoteFile->exists();
}

void notetxt::reloadRequest()
/* 
*/{
   if (! _NoteFile->open(QIODevice::ReadOnly | QIODevice::Text)){
		emit ReadError("Could not open " + _NoteFilePath);
		return;
   	}
	emit DataAvailable();
}

void notetxt::getAllText(QString* output)
/* 
*/
{
    QSettings settings;
    QTextStream in(_NoteFile);
    in.setEncoding(QStringConverter::Utf8);
    output->clear();
    while (!in.atEnd()) {
        output->append(in.readAll());
     }
	if (_NoteFile->isOpen()) _NoteFile->close();
}

void notetxt::writeRequest(QString& content)
/* Writes the content to _NoteFile
emit some signels
*/
{
   QTextStream out;
	if (! _NoteFile->open(QIODevice::WriteOnly | QIODevice::Text)){
		emit WriteError(tr("Could not open ") + _NoteFilePath);
		return;
		}
	out.setDevice(_NoteFile);
   out.setEncoding(QStringConverter::Utf8);
   out << content;

	if (_NoteFile->isOpen()){
	   _NoteFile->flush();
   	_NoteFile->close();
	}
    emit DataSaved();
    return;
}

void notetxt::setMonitoring(bool b, QObject *parent)
/* */
{
	Q_UNUSED(parent);
	if (b) {
	    	if (! _NoteFile->isOpen()){
    		emit FileSystemWatcherError();
    		return;
    		}
    	watcher = new QFileSystemWatcher();
    	watcher->removePaths(watcher->files()); // Make sure this is empty. Should only be this file we're using in this program, and only one instance
    	watcher->addPath(_NoteFilePath);
    	QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SIGNAL(DataChanged()));
		}
	else {
 	   if(watcher != NULL){
        delete watcher;
        watcher = NULL;
		}
	}
}

QString notetxt::toString()
/*
*/{
	QString ret="Note File: ";
	return ret.append(_NoteFilePath);

}
