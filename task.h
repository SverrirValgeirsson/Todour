/*
Task represent 1 single task. They can be integrated in a taskset.
The internal representation is a QString, and we extract / integrate the data.
==> this allows compatibility with todo.txt files
==> this allows compatibility with Todour view
If one needs to create another storage interface (sql, online, ...) , the separate functions can be used

License GPL
Author Gaetan DC
*/

#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QColor>
#include <QUuid>
#include <QDebug>

class task
{
private:
	QString _raw;
	QUuid _tuid;
//	QDate _d_date;
//	QDate _t_date;
	QDate _input_date;
	QDate _complete_date;
	QString _color;
	QString _priority;
//	bool _completed;
	QString _desc;
	
public:
	task(QString s="", QString context="", bool loaded=false);
	task(task* copy);
	~task();
	bool is_txt_compatible();

	void setDueDate(QString d, bool strict=true);
	void setThresholdDate(QString d, bool strict=true);
	void setInputDate(QDate d);
	void setColor(QString c);
	void setColor(QColor c);
	void setDescription(QString s);
	void setPriority(QString c);
	task* setComplete(bool c = true);
	void setRaw(QString s);

	QDate getDueDate() const;
	QDate getThresholdDate() const;
	QDate getInputDate() const;
	QString getPriority() const;
	QColor getColor() const;
	inline QUuid getTuid() const{return _tuid;}
	QString getURL() const;

	QString getRaw() const; // returns the full text, used for edit
	QString getDisplayText() const; // text for display in todour
	QString getEditText() const; // text for edit in Todour
	QString getDescription() const; // text for display in future tool. return only the descriptive part of the text, without t: due: color: utid: ...   TODO
	QString toSaveString() const;//returns the full QString for saving, including all hidden data.
	QString toString() const;//returns the full QString for debugging

	Qt::CheckState isComplete() const;
	bool isActive() const;
	

// === static functions ===
	static QDate getRelativeDate(QString d);
	static QString testRegularExpressions();
	static void taskTestSession();	
	
	inline bool operator==(task &t){return t.getTuid()==_tuid;}
	
	
protected:
	void parse(QString s, bool strict=false);

};

#endif // TASK_H
