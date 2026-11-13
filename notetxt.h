/* The tote.txt master class.
  It handles the easy note.txt file
  Created by Gdecloedt on 12/03/2026, copyright GPL
  */

#ifndef NOTETXT_H
#define NOTETXT_H

#define NOTEFILE "note.txt"

#include <QString>
#include <QFile>
#include <QFileSystemWatcher>

using namespace std;

class notetxt: public QObject
{
Q_OBJECT
public:
    explicit notetxt(QObject *parent = 0);
    ~notetxt();

	void getAllText(QString* output);
	void setMonitoring(bool b,	 QObject *parent=0);
	void reloadRequest();
	void writeRequest(QString& content);
	bool isReady();
	inline QString getType(){return "notetxt";};
	QString toString();
	
protected:    
    QString _NoteFilePath;
	QFile* _NoteFile;
	bool _ready;
 
private:
    QFileSystemWatcher *watcher;

public slots:
signals:
	void DataAvailable();
	void WriteError(QString err);
	void ReadError(QString err);
	void DataSaved();
	void DataChanged();  //What's the difference with DataAvailable???
	void FileSystemWatcherError();
};

#endif // NOTETXT_H
