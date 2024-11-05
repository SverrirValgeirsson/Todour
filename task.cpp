
#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>

static QRegularExpression todo_line("(x\\s+)?(\\([A-Z]\\)\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(.*)");
static QRegularExpression regex_url("[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_threshold_date("t:(\\d\\d\\d\\d-\\d\\d-\\d\\d) ");
static QRegularExpression regex_due_date("due:(\\d\\d\\d\\d-\\d\\d-\\d\\d) ");
static QRegularExpression regex_rec("rec:(\\w) ");
static QRegularExpression regex_color("color:([a-z]*)");
static QRegularExpression regex_priority("^\\((\\w)\\) +");
static QRegularExpression regex_done("^(x )*");
static QRegularExpression regex_threshold_short("(t:\\+\\d+[dwmypb]) "); //do not use; combine "threshold" and "reldate"
static QRegularExpression regex_due_short("(due:\\+\\d+[dwmypb]) "); //do not use; combine "due" and "reldate"
static QRegularExpression regex_rec_short("(rec:\\+?\\d+[dwmybp]) "); //do not use; combine "rec" and "reldate"
static QRegularExpression regex_threshold_project("t:(\\+[^\\s]+) ");
static QRegularExpression regex_threshold_context("t:(\\@[^\\s]+) ");
static QRegularExpression regex_reldate("\\+(\\d+)([dwmypb])");
static QRegularExpression regex_tuid("tuid:(.\\{8\\}-.\\{4\\}-.\\{4\\}-.\\{4\\}-.\\{12\\})");  //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx

bool task::is_txt_compatible()
// Some checks to do if a task can be saved as todo.txt.  usefull?
{ return true;
}

task::task(QString s)
{
	// do some checks
	// generate tuid
	// save the raw	
	// generate all the internal variables. to be externalised
	update(s);

}

task::~task()
{ // nothing
}

void task::update(QString s)
// Update the fields because the "text" has changed.
{
// Here we have to take care of the +10d (for example)    TODO
//qDebug()<<"task::update . s="<<s<<endline;
	raw=s;
	
	auto matches = regex_due_date.globalMatch(s);
	_d_date = QDate(0,12,31);
	if (matches.hasNext()) _d_date=QDate::fromString(matches.next().captured(1),"yyyy-MM-dd");	
	while (matches.hasNext())
	{
		QString str = matches.next().captured(1);
    	if (_d_date > QDate::fromString(str,"yyyy-MM-dd")) _d_date=QDate::fromString(str,"yyyy-MM-dd"); 
	}

	matches = regex_threshold_date.globalMatch(s);
	_t_date = QDate(0,12,31);
	if (matches.hasNext()) _t_date=QDate::fromString(matches.next().captured(1),"yyyy-MM-dd");	
	while (matches.hasNext())
	{
		QString str = matches.next().captured(1);
    	if (_t_date < QDate::fromString(str,"yyyy-MM-dd")) _t_date=QDate::fromString(str,"yyyy-MM-dd"); 
	}

	auto match = regex_color.match(s);
	if (match.isValid()){
		_color = QColor(match.captured(1));
}
	_priority = "";
	match = regex_priority.match(s);
	if (match.isValid()){
		_priority = match.captured(1);
		}
	
	
	if (!s.isEmpty())	_completed=(s.at(0)=='x');	
	else _completed=false;
		
}

QString task::getURL() const
{ // NOT USED
    QRegularExpressionMatch m=regex_url.match(raw);
    if(m.hasMatch()){
        //qDebug()<<"URL: "<<m.capturedTexts().at(0);
        return m.capturedTexts().at(0);
    }
    else{
        return "";
    }
}

void task::set_due_date(QDate d)
// Fonction used to set / change the due date.
// if a due-date is present, it is updated.
// if multiple due-date are present, they got cleaned
// if no due-date is present, it is added at the end
{
	raw.remove(regex_due_date);
	if (d.isValid()){
		_d_date=d;
		raw.append(" due:"+d.toString("yyyy-MM-dd"));
	}
}


void task::set_threshold_date(QDate d)
// Fonction used to set / change the t date.
// if a t-date is present, it is updated.
// if multiple t-date are present, they got cleaned
// if no t-date is present, it is added at the end
{
	raw.remove(regex_threshold_date);
	if (d.isValid()){
		_t_date=d;
		raw.append(" t:"+d.toString("yyyy-MM-dd"));
	}

}


void task::set_color(QColor c)
// Fonction used to set / change the color.
// if a color is present, it is updated.
// if multiple colors are present, they got cleaned
// if no color is present, it is added at the end
{
	raw.remove(regex_color);
	if (c.isValid())
	{
		_color=c;
		raw.append(" color:"+c.name());
	}
}


void task::set_description(QString s)
//
{
	// a définir : que contient exactement le text?
	// ceci sera sans doute la fonction la plus utilisée: on a modifié le texte de la tache.
  	//	==> Il faut tout ré-interpréter !

    Q_UNUSED(s);

	// get priority
	// get t: date (latest)
	// get due: date (latest)
	// get color
	
}


void task::set_priority(QString c)
//
{
	raw.remove(regex_priority);	
	if (!c.isEmpty())
	{
		_priority=c;
		raw.prepend("("+c+") ");
	}
}

void task::set_raw(QString s)
{
update(s);
}

void task::set_complete(bool c)
// 
{
	if (c)
	{
		raw.prepend("x ");
		_completed = true;
	}
	else
	{
		raw.remove(regex_done);
		_completed = false;
	}
}

Qt::CheckState task::isComplete() const
//
{
//	qDebug()<<"task::isComplete(): "<<(raw.at(0)=='x')<<endline;
	if (_completed) return Qt::Checked;
	else return Qt::Unchecked;
}


bool task::isActive() const
//
{
	//check thresholds:  _t_date, t:@proj, t:+context
	return (QDate::currentDate()>_t_date && !isComplete());
}

QString task::get_display_text() const
//text for display in todour, =raw - color - leading_dates - leading x
{
	return raw;
}

QString task::get_description() const
// return only the descriptive part of the text, without t: due: color: tuid: ...   TODO
{
	return raw;
} 

