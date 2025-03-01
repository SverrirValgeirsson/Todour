#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#define DATE_PAT QString("\\d\\d\\d\\d-\\d\\d-\\d\\d")
#define RDATE_PAT QString("\\+(?<n>\\d+)(?<u>[dwmypb])")

static QRegularExpression regex_date("("+DATE_PAT+")(?:\\s+|$)");
static QRegularExpression regex_reldate("(?<rdate>"+RDATE_PAT+")(?:\\s+|$)");

static QRegularExpression regex_tuid("\\s+tuid:(?<tuid>.{8}-.{4}-.{4}-.{4}-.{12})(?: |$)");  //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
static QRegularExpression regex_url("[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_color("\\s*color:([a-z]*)");
static QRegularExpression regex_done("^(?:x )+("+DATE_PAT+"(?:\\s+|$))?"); // x 2025-03-01
static QRegularExpression regex_priority("^(?:x\\s(?:"+DATE_PAT+"\\s*)?)?\\((?<prio>[A-Z])\\)(?:\\s+|$)"); //(B)
static QRegularExpression regex_inputdate("^(?:x\\s(?:"+DATE_PAT+"\\s*)?)?(?:\\([A-Z]\\)\\s+)?(?<idate>"+DATE_PAT+")(?:\\s+|$)");
static QRegularExpression regex_threshold_project("t:(\\+[^\\s]+)(?: |$)");
static QRegularExpression regex_threshold_context("t:(\\@[^\\s]+)(?: |$)");
static QRegularExpression regex_threshold_date("\\s+t:((" + DATE_PAT + "(?:\\s+|$)|" + RDATE_PAT+"))(?:\\s+|$)");
static QRegularExpression regex_due_date("\\s+due:((" + DATE_PAT + "(?:\\s+|$)|" + RDATE_PAT+"))(?:\\s+|$)");
static QRegularExpression regex_rec("(rec:" + RDATE_PAT+")(?:\\s+|$)"); 

static QRegularExpression regex_preamble("^(?:x\\s*)*(?:"+DATE_PAT+"\\s*)*(?:\\([A-Z]\\)\\s*)*(?:"+DATE_PAT+")*(?:\\s+|$)");

static QRegularExpression regex_todo_line("((^(?:x )+("+DATE_PAT+"(?:\\s+|$))?)+(?<donedate>"+DATE_PAT+"(?:\\s+|$))?)?(?<priority>"+regex_priority.pattern()+")?(?<inputdate>"+DATE_PAT+"(?:\\s+|$))?(.*)");


bool task::is_txt_compatible()
/* Some checks to do if a task can be saved as todo.txt.  usefull?
   #IDEA: can make use of regex_todo_line ?
*/
{ return true;
}

task::task(QString s, bool loaded)
/* Create a task based on input String. 
	If loaded=true, don't assume any default (task is loaded from file)
*/
{
//	qDebug()<<"task:task Task constructor s="<<s<<endline;

	update(s.replace('\n',' '),true);
	


QString ret=s;


//- TUID, inputdate, default prio.
	if (loaded) //we loaded from the file, we should find the data in task
	{
		auto match = regex_tuid.match(s);
		if (match.hasMatch())
		{
			tuid= QUuid::fromString(match.captured("tuid"));
			ret.remove(regex_tuid);
		}
		match = regex_inputdate.match(s);
		if (match.hasMatch())
		{
			_input_date = QDate::fromString(match.captured("idate"),"yyyy-MM-dd");
		}
	}
	else //this is a new typed task, default inputdate if settings + tuid + ??
	{
		QSettings settings;	

    	if(settings.value(SETTINGS_DATES).toBool()){
    		 _input_date=QDate::currentDate();
    		 //ret.prepend(_input_date.toString("yyyy-MM-dd")+" ");
    		 }
	
		if (_priority.isEmpty()){ // there was no priority in the newly created task
			//qDebug()<<"We put a default priority"<<endline;
			_priority = settings.value(SETTINGS_DEFAULT_PRIORITY,DEFAULT_DEFAULT_PRIORITY).toString();
    		}
    	
		tuid=QUuid::createUuid();
	}



	
	// do some checks
	// generate tuid
	// save the _raw

	update(ret.replace('\n',' '),true);
}

task::~task()
{ // nothing
}

void task::update(QString s,bool strict)
/* Update the fields because the "text" has changed.
*/
{
//qDebug()<<"task::update . s="<<s<<endline;
	_raw=s;

	auto matches = regex_due_date.globalMatch(s);
	while (matches.hasNext())
	{
		this->set_due_date(matches.next().captured(1),strict);
	}

	matches = regex_threshold_date.globalMatch(s);
	while (matches.hasNext())
	{
		this->set_threshold_date(matches.next().captured(1),strict);
	}
	
	auto match = regex_color.match(s);
	if (match.hasMatch()){
		this->set_color(match.captured(1));
	}
	_raw.remove(regex_color);

//	_priority = "";
	match = regex_priority.match(s);
	if (match.hasMatch()){
	//qDebug()<<"we found priority: "<<match.captured("prio")<<endline;
		this->set_priority(match.captured("prio"));
		}
		

	match = regex_done.match(s);
	if (match.hasMatch()){
		_completed = true;
		}
	else
	{
		_completed = false;
	}
	_raw.remove(regex_preamble);
}

void task::set_due_date(QString d, bool strict)
/* Update the due_date from String, with interpretation.
   We expect due:2024-11-25 or due:+2d
  if strict=false, change _d_date only if new date is sooner
*/
{
//qDebug()<<"task::set_due_date input="<<d<<endline;
	QDate td;
	QRegularExpressionMatch rdate = regex_date.match(d);
	if (rdate.hasMatch())
	{
		td = QDate::fromString(d,"yyyy-MM-dd");
	}
	rdate = regex_reldate.match(d);
	if (rdate.hasMatch())
	{
		td = task::getRelativeDate(rdate.captured(0));

	}
	if (td.isValid()){ //if new date is not valid, do nothing.
			_raw.remove(regex_due_date);
			_d_date=td;
			_raw.append(" due:"+td.toString("yyyy-MM-dd"));
			
	}
}


void task::set_threshold_date(QString d, bool strict)
/* Update the threshold_date from String, with interpretation.
   We expect t:2024-11-25 or t:+2d
  if strict=false, change _t_date only if new date is later
*/
{
//qDebug()<<d<<endline;
	QDate td;
	QRegularExpressionMatch rdate = regex_date.match(d);
	if (rdate.hasMatch())
	{
		td = QDate::fromString(d,"yyyy-MM-dd");
	}
	rdate = regex_reldate.match(d);
	if (rdate.hasMatch())
	{
		td = task::getRelativeDate(rdate.captured(0));
	}

	if (td.isValid()){ //if new date is not valid, do nothing
			_raw.remove(regex_threshold_date);
			_t_date=td;
			_raw.append(" t:"+td.toString("yyyy-MM-dd"));
	}
}

void task::set_color(QString c)
/* Fonction used to set / change the color.
 if a color is present, it is updated.
 if multiple colors are present, they got cleaned
 */
{
	_raw.remove(regex_color);
	_color=c;
	}


void task::set_description(QString s)
/*
NOT WORKING IN THIS STATE
#IDEA: we should rebuilt the _raw, based on the new "description", + all the fields that we have.
*/
{
	// a définir : que contient exactement le text?
	// ceci sera sans doute la fonction la plus utilisée: on a modifié le texte de la tache.
  	//	==> Il faut tout ré-interpréter !
    Q_UNUSED(s);
}


void task::set_priority(QString c)
/* Set the priority of task
#BUG: il faut enlever tout ce qui ressemble à une priorité. Si il y en a 2 d'affilée, on enleve les 2.
*/
{

	_raw.remove(regex_priority);	
	if (!c.isEmpty())
	{
		_priority=c;
		_raw.prepend("("+c+") ");
	}

}

void task::set_raw(QString s)
/* Set raw text, by first interpreting it.
*/
{
	update(s);
}

task* task::set_complete(bool c)
/* Mark the task as completed.
 If there is any rec: pattern,
   - duplicate the task with a new threshold (or due)
   - mark the current complete.
*/ 
{
	QSettings settings;
	_complete_date = QDate::currentDate();
	task *ret=0;
	_raw.remove(regex_done);

	if (c){
		auto matches = regex_rec.globalMatch(_raw);
		while (matches.hasNext())
		{
			ret = new task(_raw);
			if (settings.value(SETTINGS_DEFAULT_THRESHOLD,DEFAULT_DEFAULT_THRESHOLD) == "t:"){
				ret->set_threshold_date(matches.next().captured(1),true);
			}else{	
				ret->set_due_date(matches.next().captured(1),true);
			}
		}
		_completed = true;
	}else{
		_completed = false;
	}
	return ret;
}

QString task::getURL() const
/*
*/
{ // NOT USED
    QRegularExpressionMatch m=regex_url.match(_raw);
    if(m.hasMatch()){
        //qDebug()<<"URL: "<<m.capturedTexts().at(0);
        return m.capturedTexts().at(0);
    }
    else{
        return "";
    }
}

QString task::get_edit_text() const
/* returns text for display in todour, = leading x + priority + dates? + _raw + color
*/
{
	QSettings settings;
	QString ret = _raw;
	if (! _color.isEmpty())
			ret.append(" color:"+_color);
	
	if (settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool())
			ret.prepend(_input_date.toString("yyyy-MM-dd")+" ");
			
	if (! _priority.isEmpty())
			ret.prepend("("+_priority+") ");
	
	if (settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool())
		{
		if (_complete_date.isValid())
				ret.prepend(_complete_date.toString("yyyy-MM-dd")+" ");
		}
	
	if (_completed)
			ret.prepend("x ");
	
	return ret;
}



QString task::get_display_text() const
/* returns text for display in todour, = priority + dates? + _raw
*/
{
	QSettings settings;
	QString ret = _raw;
	ret.remove(regex_preamble);
	
	if (settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool())
			ret.prepend(_input_date.toString("yyyy-MM-dd")+" ");
			
	if (! _priority.isEmpty())
			ret.prepend("("+_priority+") ");
	
	return ret;
}

QString task::get_description() const
/* returns only the descriptive part of the text, without t: due: color:
*/
{
	QString ret = _raw;
	ret.remove(regex_color);
	ret.remove(regex_done);
	ret.remove(regex_threshold_date);
	ret.remove(regex_due_date);
	ret.remove(regex_rec);
	return ret;
} 

QString task::toString() const
/* returns the full QString for saving, including all hidden data.
*/
{
	QString ret=get_edit_text();
	ret.append(" tuid:"+tuid.toString(QUuid::WithoutBraces));
	return ret;
}

bool task::isActive() const
/* returns true if the task is active. Active means:
	- threshold date not greater than today
	- task not completed
	- context threshold not activated (!)
	- not containing "inactive" tags (LATER: IDEA: LOW: WAIT: ..)
*/
{
	QSettings settings;
	bool ret=true;
	ret &= (QDate::currentDate()>_t_date);
	ret &= !_completed;
	QStringList words = settings.value(SETTINGS_INACTIVE,DEFAULT_INACTIVE).toString().split(';');
	for (QString i:words)
		{
		ret &= ! _raw.contains(i);
		}
return ret;
}

/*
====================================  STATIC FUNCTIONS ========================================
*/
QDate task::getRelativeDate(QString d)
/* static function, returns a QDate object based on the "relative" as described in regex_reldate
*/
{
	QSettings settings;
    QDate ret = QDate::currentDate();
    QRegularExpressionMatch m = regex_reldate.match(d);
//	qDebug()<<"task::getRelativeDate match:"<<m.captured("rdate")<<endline;
    if (!m.hasMatch()) return QDate();
        if(m.captured("u").contains('d')){
            ret = ret.addDays(m.captured("n").toInt());
        } else if(m.captured("u").contains('w')){
            ret = ret.addDays(m.captured("n").toInt()*7);
        } else if(m.captured("u").contains('m')){
            ret = ret.addMonths(m.captured("n").toInt());
        } else if(m.captured("u").contains('y')) {
            ret = ret.addYears(m.captured("n").toInt());
        } else if(m.captured("u").contains('p')){
            // Ok. This is the procrastination 'feature'. Add a random number of days and also say that this was procrastrinated
            ret = ret.addDays(rand()%m.captured("n").toInt()+1);
            //extra = " +procrastinated";
        } else if (m.captured("u").contains('b')){
            // Business days. Naive implementation
            QSettings settings;
            QList<int> business_days = settings.value(SETTINGS_BUSINESS_DAYS, QVariant::fromValue(QList<int>())).value<QList<int> >();
            if(business_days.size()==0){
                // Hard code some defaults
                for(int i=DEFAULT_BUSINESS_DAYS_FIRST;i<=DEFAULT_BUSINESS_DAYS_LAST;i++){
                    business_days<<i;
                }
            }
            // 1=Monday, 6=Sat, 7=sun
            int days=0;
            int add = m.captured("n").toInt();
            while(days<add){
                ret = ret.addDays(1); // add one at a time
                if(business_days.contains(ret.dayOfWeek())){
                    days++;
                }
            }
        }
        //return d.toString("yyyy-MM-dd")+extra;
        return ret;
}

QString task::testRegularExpressions()
/* static function, make a test on the RegularExpressions.
Only for development, not planned to be used for production.
*/
{
	if(! regex_todo_line.isValid()) return "regex_todo_line "+regex_todo_line.errorString();
	if(! regex_tuid.isValid()) return "regex_tuid "+regex_tuid.errorString();
	if(! regex_reldate.isValid()) return "regex_reldate "+regex_reldate.errorString();
	if(! regex_url.isValid()) return "regex_url "+regex_url.errorString();
	if(! regex_threshold_date.isValid()) return "regex_threshold_date "+regex_threshold_date.errorString();
	if(! regex_due_date.isValid()) return "regex_due_date "+regex_due_date.errorString();
	if(! regex_rec.isValid()) return "regex_rec "+regex_rec.errorString();
	if(! regex_color.isValid()) return "regex_color "+regex_color.errorString();
	if(! regex_priority.isValid()) return "regex_priority "+regex_priority.errorString();
	if(! regex_done.isValid()) return "regex_done "+regex_done.errorString();
	if(! regex_threshold_project.isValid()) return "regex_threshold_project "+regex_threshold_project.errorString();
	if(! regex_threshold_context.isValid()) return "regex_threshold_context "+regex_threshold_context.errorString();
return "testRegularExpressions finished";
}


void task::taskTestSession()
{
	qDebug()<<"Testing regularExpression : "<<task::testRegularExpressions()<<endline;
 //test zone

qDebug()<<"Test zone: "<<endline;
task *t = new task ("(B) This is a full test task t:2024-10-09 due:2024-11-01 due:+1m color:red");
task *u = new task ("This is a second test task t:2025-10-09");
task *v = new task ("x This is a finished test task t:2022-10-01");
qDebug()<<"Created. Let's test:"<<endline;

qDebug()<<"t raw: "<<t->get_raw()<<endline;
qDebug()<<"t due-date: "<<t->get_due_date()<<endline;
qDebug()<<"t threshold-date: "<<t->get_threshold_date()<<endline;
qDebug()<<"t text: "<<t->get_display_text()<<endline;
//qDebug()<<"t pure text: "<<t->get_pure_text()<<endline;
qDebug()<<"t priority: "<<t->get_priority()<<endline;
qDebug()<<"t color: "<<t->get_color()<<endline;
qDebug()<<"t is Complete: "<<t->isComplete()<<endline;
qDebug()<<"t is Active: "<<t->isActive()<<endline;
//qDebug()<<"t utid: "<<t->get_utid()<<endline;


t->set_priority("A");
qDebug()<<"t raw: "<<t->toString()<<endline;
t->set_priority("Z");
qDebug()<<"t raw: "<<t->toString()<<endline;

qDebug()<<"u raw: "<<u->toString()<<endline;
qDebug()<<"u due-date: "<<u->get_due_date()<<endline;
qDebug()<<"u threshold-date: "<<u->get_threshold_date()<<endline;
qDebug()<<"u text: "<<u->get_display_text()<<endline;
//qDebug()<<"u pure text: "<<u->get_pure_text()<<endline;
qDebug()<<"u priority: "<<u->get_priority()<<endline;
qDebug()<<"u color: "<<u->get_color()<<endline;
qDebug()<<"u is Complete: "<<u->isComplete()<<endline;
qDebug()<<"u is Active: "<<u->isActive()<<endline;
//qDebug()<<"u utid: "<<u->get_utid()<<endline;

qDebug()<<"v raw: "<<v->toString()<<endline;
qDebug()<<"v due-date: "<<v->get_due_date()<<endline;
qDebug()<<"v threshold-date: "<<v->get_threshold_date()<<endline;
qDebug()<<"v text: "<<v->get_display_text()<<endline;
//qDebug()<<"v pure text: "<<v->get_pure_text()<<endline;
qDebug()<<"v priority: "<<v->get_priority()<<endline;
qDebug()<<"v color: "<<v->get_color()<<endline;
qDebug()<<"v is Complete: "<<v->isComplete()<<endline;
qDebug()<<"v is Active: "<<v->isActive()<<endline;
//qDebug()<<"v utid: "<<v->get_utid()<<endline;

qDebug()<<"End of test session"<<endline;


}
