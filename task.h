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
//#include <QDate>
#include <QColor>
#include <QUuid>
#include <QDebug>
#include <QDateTime>

class task
{
private:
	QString _raw;
	QUuid _tuid;
	QDateTime _ttag;
	QDateTime _complete_date;
	QChar priority;
	QColor color;
	QDateTime inputD;
	QDateTime dueD;
	QDateTime thrD;
	Qt::CheckState complete ;
	bool active;
	
public:
	task(QString s="", QString context="", bool loaded=false);
	task(task* copy);
	task(QString s, QUuid tuid);

	~task();
	bool is_txt_compatible();

	void setDueDate(QDateTime d);
	
	void setThresholdDate(QDateTime d);
	void setInputDate(QDateTime d);
	
	void setColor(QString c);
	void setColor(QColor c);
	
	void setDescription(QString s);
	void setPriority(QChar c);
	task* setComplete(bool c = true);
	void setRaw(QString s);

	void forceActive(bool state);
	
	inline QDateTime const *getDueDate() {return &dueD;};
	inline QDateTime const *getThresholdDate() {return &thrD;};
	inline QDateTime const *getInputDate() {return &inputD;};
	inline QChar const getPriority() {return priority;};
	inline QColor const *getColor() {return &color;};
	inline QUuid getTuid() const{return _tuid;}
	inline Qt::CheckState isComplete() const {return complete;};
	/*  */
	
	void refreshActive(QDateTime now);
	
	inline bool isActive() const {return active;};
	/* */

	QString getURL() const;

	inline QString getRaw() const {return _raw;};
	/* returns the full text, used for edit */
	
	QString getDisplayText() const;
	/* text for display in todour*/
	
	inline QString getEditText() const {return _raw;};
	/* text for edit in Todour*/
	
	QString getDescription() const;
	/* text for display in future tool. return only the descriptive part of the text, without t: due: color: utid: ...   TODO*/
	
	QString toSaveString() const;
	/*returns the full QString for saving, including all hidden data.*/
	
	QString toSaveString_pureTODO() const;
	/*returns the full QString for saving, including all hidden data.*/
	
	QString toString() const;
	/*returns the full QString for debugging*/

	inline QDateTime getTimeStamp() const {return _ttag;}



// === static functions ===
	static QDateTime getRelativeDate(QString d, const QDateTime *base = nullptr);
	static void taskTestSession();	
	
	inline bool operator==(task &t){return t.getTuid()==_tuid;}
	
	
protected:
	void parse(QString s, bool strict=false);

	
	
private:
};

#endif // TASK_H
