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

class task
{
private:
	QString raw;
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
	void set_text(QString s);
	void set_priority(QString c);
	void set_complete(bool c);

	void update(QString s);

	QDate get_due_date() const;
	QDate get_threshold_date() const;
	QString get_priority() const;
	QColor get_color() const;
	Qt::CheckState isComplete() const;
	bool isActive() const;
	uint get_utid() const;
	QString getURL() const;
	
	QString get_raw() const; // used to export to todo_txt file
	QString get_text_long() const;//text for edit in todour, =raw - leading dates - tuid
	QString get_text() const; // text for display in todour, =text_long - color - etc
	QString get_pure_text() const; // text for display in future tool. return only the descriptive part of the text, without t: due: color: utid: ...   TODO

	
};

#endif // TASK_H
