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

class task
{
private:
	QString raw;
	QUuid tuid;
	QDate _d_date;
	QDate _t_date;
	QColor _color;
	QString _priority;
	bool _completed;
	
public:
	task(QString s);
	~task();
	bool is_txt_compatible();

	void set_due_date(QDate d);
	void set_threshold_date(QDate d);
	void set_color(QColor c);
	void set_description(QString s);
	void set_priority(QString c);
	void set_complete(bool c = true);
	void set_raw(QString s);

	inline QDate get_due_date() const {return _d_date;}
	inline QDate get_threshold_date() const{return _t_date;}

	inline QString get_priority() const{return _priority;}
	inline QColor get_color() const{return _color;}
	Qt::CheckState isComplete() const;
	bool isActive() const;
	inline QString get_tuid() const{return tuid.toString();}
	QString getURL() const;
	
	inline QString get_raw() const{return raw;} // used to export to todo_txt file
	QString get_display_text() const; // text for display in todour, =text_long - color - etc
	QString get_description() const; // text for display in future tool. return only the descriptive part of the text, without t: due: color: utid: ...   TODO

protected:
	void update(QString s);


};

#endif // TASK_H
