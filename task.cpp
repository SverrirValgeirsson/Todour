#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#define DATE_PAT QString("\\d\\d\\d\\d-\\d\\d-\\d\\d")
#define RDATE_PAT QString("\\+(?<n>\\d+)(?<u>[dwmypb])")

static QRegularExpression regex_date("("+DATE_PAT+")(?:\\s*|$)");
static QRegularExpression regex_reldate("(?<rdate>"+RDATE_PAT+")(?:\\s*|$)");

static QRegularExpression regex_tuid("\\s+tuid:(?<tuid>.{8}-.{4}-.{4}-.{4}-.{12})(?:\\s*|$)");  //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
static QRegularExpression regex_url("[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_color("\\s*color:([a-z]*)");
static QRegularExpression regex_priority("^(?:x\\s(?:"+DATE_PAT+"\\s*)?)?\\((?<prio>[A-Z])\\)(?:\\s*|$)"); //(B)
static QRegularExpression regex_inputdate("(?<idate>\\b"+DATE_PAT+")\\s+|$");
static QRegularExpression regex_threshold_project("t:(\\+[^\\s]+)(?:\\s*|$)");
static QRegularExpression regex_threshold_context("t:(\\@[^\\s]+)(?:\\s*|$)");
static QRegularExpression regex_threshold_date("\\s+t:((" + DATE_PAT + "(?:\\s*|$)|" + RDATE_PAT+"))(?:\\s*|$)");
static QRegularExpression regex_due_date("\\s+due:(" + DATE_PAT + ")(?:\\s+|$)");
static QRegularExpression regex_rec("(rec:" + RDATE_PAT+")(?:\\s*|$)"); 

static QRegularExpression regex_done        ("^(?:x )+("+DATE_PAT+"(?:\\s+|$))?"); // x 2025-03-01
static QRegularExpression regex_completedate("^(?:x )+("+DATE_PAT+"(?:\\s+|$))");

static QRegularExpression regex_preamble("^(?:x\\s*)*(?:"+DATE_PAT+"\\s*)*(?:\\([A-Z]\\)\\s*)*(?:"+DATE_PAT+")*(?:\\s*|$)");

static QRegularExpression regex_todo_line("((^(?:x )+("+DATE_PAT+"(?:\\s+|$))?)+(?<donedate>"+DATE_PAT+"(?:\\s*|$))?)?(?<priority>"+regex_priority.pattern()+")?(?<inputdate>"+DATE_PAT+"(?:\\s*|$))?(.*)");


bool task::is_txt_compatible()
/* Some checks to do if a task can be saved as todo.txt.  usefull?
   #IDEA: can make use of regex_todo_line ?
*/
{ return true;
}

task::task(QString s, QString context, bool loaded)
/* Create a task based on input String. 
	If loaded=true, don't assume any default (task is loaded from file)
	Context is a base info to be added.
*/
{
//	qDebug()<<"task:task Task constructor s="<<s<<"  context="<<context<<"  loaded="<<loaded<<endline;
	QString ret=s;
	if (loaded) //we loaded from the file, we should find the data in task
	{
		auto match = regex_tuid.match(s);
		if (match.hasMatch())
		{
			_tuid= QUuid::fromString(match.captured("tuid"));
			ret.remove(regex_tuid);
			if (_tuid.isNull())
					_tuid=QUuid::createUuid();
		}
		parse(ret,true);
	}
	else //this is a new typed task, default inputdate if settings + tuid + ??
	{
		QSettings settings;	
		
		_tuid=QUuid::createUuid();

		if (!context.isEmpty())
			ret = ret+" "+context;
		parse(ret,true);

    	if(!getInputDate().isValid()&&(settings.value(SETTINGS_DATES).toBool())){
    		 setInputDate(QDate::currentDate());
    		 }
	
		if (getPriority().isEmpty()){ // there was no priority in the newly created task
			setPriority(settings.value(SETTINGS_DEFAULT_PRIORITY,DEFAULT_DEFAULT_PRIORITY).toString());			
    		}
	}

}

task::task(task* copy)
{
	setRaw(copy->getRaw());
	_tuid=copy->getTuid();
//	qDebug()<<"task:task from existing. raw="<<_raw<<endline;	
	parse(_raw,true); // usefull? should not be ;-)
}

task::~task()
{ // nothing to do
}

void task::parse(QString s,bool strict)
/* Update the fields because the "text" has changed.  Only tak care of relative dates...
*/
{
	_raw=s;

	auto matches = regex_due_date.globalMatch(s);
	while (matches.hasNext())
	{
		this->setDueDate(matches.next().captured(1),strict);
	}

	matches = regex_threshold_date.globalMatch(s);
	while (matches.hasNext())
	{
		this->setThresholdDate(matches.next().captured(1),strict);
	}
	

}

void task::setDueDate(QString d, bool strict)
/* Update the due_date from String, with interpretation.
   We expect due:2024-11-25 or due:+2d
  if strict=false, change _d_date only if new date is sooner
*/
{
//qDebug()<<"task::setDueDate input="<<d<<endline;
	QDate td;
	td = task::getRelativeDate(d);
	if(! td.isValid()){
		QRegularExpressionMatch rdate = regex_date.match(d);
		if (rdate.hasMatch())
		{
			td = QDate::fromString(d,"yyyy-MM-dd");
		}
	}
	if (td.isValid()){ //if new date is not valid, do nothing.
			_raw.remove(regex_due_date);
//			_d_date=td;
			_raw.append(" due:"+td.toString("yyyy-MM-dd"));
			
	}
}


void task::setThresholdDate(QString d, bool strict)
/* Update the threshold_date from String, with interpretation.
   We expect t:2024-11-25 or t:+2d
  if strict=false, change _t_date only if new date is later
*/
{
//qDebug()<<d<<endline;
	QDate td;
	td = task::getRelativeDate(d);
	if(! td.isValid()){
		QRegularExpressionMatch rdate = regex_date.match(d);
		if (rdate.hasMatch())
		{
			td = QDate::fromString(d,"yyyy-MM-dd");
		}
	}
	if (td.isValid()){ //if new date is not valid, do nothing
			_raw.remove(regex_threshold_date);
//			_t_date=td;
			_raw.append(" t:"+td.toString("yyyy-MM-dd"));
	}
}

void task::setInputDate(QDate d)
/* place a date at beginning, but after priority.*/
{
	//qDebug()<<"task::set_nputdate, raw="<<_raw<<"  d="<<d<<endline;
	if (!getPriority().isEmpty())
		_raw.insert(4,d.toString("yyyy-MM-dd")+" ");
	else 
		_raw.insert(0,d.toString("yyyy-MM-dd")+" ");
	//qDebug()<<"task::setInputDate, raw="<<_raw<<endline;

}

void task::setColor(QString c)
/* set / change the color.  if a color is present, it is updated,  if multiple colors are present, they got cleaned
 */
{
	_raw.remove(regex_color);
	_color=c;
}

void task::setColor(QColor c)
/* No direct function for this in QT, must be rewritten...  :-(*/
{
qDebug()<<"   task::setColor not implemented"<<endline;
	Q_UNUSED(c);
	return;
}

void task::setDescription(QString s)
/*
NOT WORKING IN THIS STATE
#IDEA: we should rebuilt the _raw, based on the new "description", + all the fields that we have.
*/
{
qDebug()<<"   task::setDescription not implemented"<<endline;
	// a définir : que contient exactement le text?
	// ceci sera sans doute la fonction la plus utilisée: on a modifié le texte de la tache.
  	//	==> Il faut tout ré-interpréter !
    Q_UNUSED(s);
}


void task::setPriority(QString c)
/* Set the priority of task
#BUG: il faut enlever tout ce qui ressemble à une priorité. Si il y en a 2 d'affilée, on enleve les 2.
*/
{
	_raw.remove(regex_priority);	
	if (!c.isEmpty())
			_raw.prepend("("+c+") ");
}

void task::setRaw(QString s)
/* Set raw text, by first interpreting it.
*/
{
	_raw=s;
	parse(s);
}

Qt::CheckState task::isComplete() const
/* */
{
	auto matches = regex_done.globalMatch(_raw);
	while (matches.hasNext())
	{
		return Qt::Checked;
	}
	return Qt::Unchecked;
}


task* task::setComplete(bool c)
/* Mark the task as completed.
 If there is any rec: pattern,
   - duplicate the task with a new threshold (or due)
   - mark the current complete.
*/ 
{
	QSettings settings;
//	_complete_date = QDate::currentDate();
	task *ret=0;
//	_raw.remove(regex_done);
	if (c){
		auto matches = regex_rec.globalMatch(_raw);
		while (matches.hasNext())
		{
			ret = new task(_raw); // we use the normal constructor to get a new tuid.
			if (settings.value(SETTINGS_DEFAULT_THRESHOLD,DEFAULT_DEFAULT_THRESHOLD) == "t:"){
				ret->setThresholdDate(matches.next().captured(1),true);
			}else{	
				ret->setDueDate(matches.next().captured(1),true);
			}
		}
    if(settings.value(SETTINGS_DATES).toBool())
 			_raw.prepend("x "+QDate::currentDate().toString("yyyy-MM-dd")+" ");
 	else
 			_raw.prepend("x ");
	}
	else{
		_raw.remove(regex_done);
	}
	
	return ret;
}

QString task::getURL() const
/* */
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

QString task::getRaw() const
/* returns the full text, used for copying. TUID not included. */
{
  return _raw;
}


QString task::getEditText() const
/* returns text for display in todour, = priority + dates? + _raw + color       = raw ? 
*/
{
	return _raw;
}


QString task::getDisplayText() const
/* returns text for display in todour, = priority + dates? + _raw
	returns raw - complete - dates? - color
*/
{
	QSettings settings;
	QString ret = _raw;
	
	if (! settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool()){
		ret.remove(regex_inputdate);
		ret.remove(regex_completedate);
		}

	ret.remove(regex_color);
	ret.remove(regex_done);
	
	return ret;
}

QString task::getDescription() const
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

QDate task::getDueDate() const
/* */
{
	QDate d, e;
	auto matches = regex_due_date.globalMatch(_raw);
	while (matches.hasNext())
	{
		e=QDate::fromString(matches.next().captured(1),"yyyy-MM-dd");
		if (e.isValid() && (!d.isValid() || d<e))
			d=e;		
	
	}
return d;
}

QDate task::getThresholdDate() const
/* */
{
	QDate d, e;
	auto matches = regex_threshold_date.globalMatch(_raw);
	while (matches.hasNext())
	{
		e=QDate::fromString(matches.next().captured(1),"yyyy-MM-dd");
		if (e.isValid() && (!d.isValid() || d<e))
				d=e;	
	}
return d;
}


QDate task::getInputDate() const
/* */
{
	QDate d;
	auto matches = regex_inputdate.globalMatch(_raw);
	while (matches.hasNext())
	{
		d=QDate::fromString(matches.next().captured(1),"yyyy-MM-dd");
		if (d.isValid()) return d;
	}
	return QDate();
}

QString task::getPriority() const
/**/
{
	QString c;
	
	auto matches = regex_priority.globalMatch(_raw);
	while (matches.hasNext())
	{
		return matches.next().captured(1);
	}

	return "";
}

QColor task::getColor() const
/* */
{
	QColor c;
	
	auto matches = regex_color.globalMatch(_raw);
	while (matches.hasNext())
	{
		if (QColor::isValidColorName(matches.next().captured(1))){
			return QColor::fromString(matches.next().captured(1));
		}
	}
	return QColor::fromString("White");
}



QString task::toSaveString() const
/* returns the full QString for saving, including all hidden data.
*/
{
	QString ret=getEditText();
	ret.append(" tuid:"+_tuid.toString(QUuid::WithoutBraces));
	return ret;
}

QString task::toString() const
/* returns the full QString for debugging, including all hidden data.
*/
{
	QString ret=getEditText();
	ret.append("\n    tuid:"+_tuid.toString(QUuid::WithoutBraces));
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
	ret &= (QDate::currentDate()>getThresholdDate());
//	ret &= !isComplete();
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

qDebug()<<"t raw: "<<t->getRaw()<<endline;
qDebug()<<"t due-date: "<<t->getDueDate()<<endline;
qDebug()<<"t threshold-date: "<<t->getThresholdDate()<<endline;
qDebug()<<"t text: "<<t->getDisplayText()<<endline;
//qDebug()<<"t pure text: "<<t->get_pure_text()<<endline;
qDebug()<<"t priority: "<<t->getPriority()<<endline;
qDebug()<<"t color: "<<t->getColor()<<endline;
qDebug()<<"t is Complete: "<<t->isComplete()<<endline;
qDebug()<<"t is Active: "<<t->isActive()<<endline;
//qDebug()<<"t utid: "<<t->getTuid()<<endline;


t->setPriority("A");
qDebug()<<"t raw: "<<t->toString()<<endline;
t->setPriority("Z");
qDebug()<<"t raw: "<<t->toString()<<endline;

qDebug()<<"u raw: "<<u->toString()<<endline;
qDebug()<<"u due-date: "<<u->getDueDate()<<endline;
qDebug()<<"u threshold-date: "<<u->getThresholdDate()<<endline;
qDebug()<<"u text: "<<u->getDisplayText()<<endline;
//qDebug()<<"u pure text: "<<u->get_pure_text()<<endline;
qDebug()<<"u priority: "<<u->getPriority()<<endline;
qDebug()<<"u color: "<<u->getColor()<<endline;
qDebug()<<"u is Complete: "<<u->isComplete()<<endline;
qDebug()<<"u is Active: "<<u->isActive()<<endline;
//qDebug()<<"u utid: "<<u->getTuid()<<endline;

qDebug()<<"v raw: "<<v->toString()<<endline;
qDebug()<<"v due-date: "<<v->getDueDate()<<endline;
qDebug()<<"v threshold-date: "<<v->getThresholdDate()<<endline;
qDebug()<<"v text: "<<v->getDisplayText()<<endline;
//qDebug()<<"v pure text: "<<v->get_pure_text()<<endline;
qDebug()<<"v priority: "<<v->getPriority()<<endline;
qDebug()<<"v color: "<<v->getColor()<<endline;
qDebug()<<"v is Complete: "<<v->isComplete()<<endline;
qDebug()<<"v is Active: "<<v->isActive()<<endline;
//qDebug()<<"v utid: "<<v->getTuid()<<endline;

qDebug()<<"End of test session"<<endline;


}
