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
	QUuid tuid;
	QDate _d_date;
	QDate _t_date;
	QDate _input_date;
	QDate _complete_date;
	QString _color;
	QString _priority;
	bool _completed;
	
public:
	task(QString s, bool loaded=false);
	~task();
	bool is_txt_compatible();

	void set_due_date(QString d, bool strict=true);
	void set_threshold_date(QString d, bool strict=true);
	void set_color(QString c);
	void set_description(QString s);
	void set_priority(QString c);
	task* set_complete(bool c = true);
	void set_raw(QString s);

	inline QDate get_due_date() const {return _d_date;}
	inline QDate get_threshold_date() const{return _t_date;}
	inline QDate get_input_date() const {return _input_date;}
	inline QString get_priority() const{return _priority;}
	inline QColor get_color() const{if (QColor::isValidColorName(_color)){return QColor::fromString(_color);}return QColor::fromString("White");}
	inline QUuid get_tuid() const{return tuid;}
	QString getURL() const;

	inline QString get_raw() const{qDebug()<<"Using deprecated get_raw()"; return _raw;} // returns the full text, used for edit
	QString get_display_text() const; // text for display in todour
	QString get_edit_text() const; // text for edit in Todour
	QString get_description() const; // text for display in future tool. return only the descriptive part of the text, without t: due: color: utid: ...   TODO
	QString toString() const;

	inline Qt::CheckState isComplete() const{	if (_completed) return Qt::Checked;	else return Qt::Unchecked;};
	bool isActive() const;
	

// === static functions ===
	static QDate getRelativeDate(QString d);
	static QString testRegularExpressions();
	static void taskTestSession();

//trash
	inline bool isActive(QDate today) const{return (today>_t_date && !_completed);}; // An active task has threshold in the past, and is not completed. Correct???

protected:
	void update(QString s, bool strict=false);


};

#endif // TASK_H
