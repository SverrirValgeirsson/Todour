#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

#define DATE_PAT QString("\\d\\d\\d\\d-\\d\\d-\\d\\d")
#define RDATE_PAT QString("\\+(?<n>\\d+)(?<u>[dwmypb])")

static QRegularExpression regex_date("("+DATE_PAT+")(?:\\s*|$)");
static QRegularExpression regex_reldate("(?<rdate>"+RDATE_PAT+")(?:\\s*|$)");

static QRegularExpression regex_url("(?:\\s+)[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_color("(?:\\s+)color:([a-z]*)");
static QRegularExpression regex_threshold_project("(?:\\s+)t:(\\+[^\\s]+)(?#\\s+|$)");
static QRegularExpression regex_threshold_context("(?:\\s+)t:(\\@[^\\s]+)(?#\\s+|$)");
static QRegularExpression regex_threshold_date("(?:\\s+)(t:" + DATE_PAT + ")(?#\\s+|$)");
static QRegularExpression regex_threshold_date_r("(?:\\s+)(t:" + RDATE_PAT + ")(?#\\s+|$)");

static QRegularExpression     regex_due_date("(?:\\s+)(due:" + DATE_PAT + ")(?#\\s+|$)");
static QRegularExpression     regex_due_date_r("(?:\\s+)(due:" + RDATE_PAT + ")(?#\\s+|$)");

static QRegularExpression regex_rec("(?:\\s+)(rec:" + RDATE_PAT+")(?#\\s+|$)"); 

//preamble    //^(?:x\s+(?:\d\d\d\d-\d\d-\d\d\s)?)?(?:\(\w\)\s+)?(?:\d\d\d\d-\d\d-\d\d\s+)?
static QRegularExpression regex_preamble("^(?<pxd>x\\s+"+DATE_PAT+"\\s)?(?<px>x\\s)?\\s*(?<ppr>\\(\\w\\)\\s)?\\s*(?<pid>"+DATE_PAT+"\\s)?\\s*(?<ptk>.*)$");
static QRegularExpression regex_done        ("^(x\\s+)"); // x 2025-03-01
static QRegularExpression regex_completedate("^(x\\s+("+DATE_PAT+")?\\s+)");
static QRegularExpression regex_priority("^(?:x\\s+(?="+DATE_PAT+")?)?(\\(\\w\\)\\s+)"); //(B) 2025-05-05
static QRegularExpression regex_inputdate("^(?:x\\s)?(?:x\\s+"+DATE_PAT+"\\s)?\\s*(?:\\(\\w\\)\\s)?\\s*(?<p>"+DATE_PAT+")");

static QRegularExpression regex_todo_line("((^(?:x )+("+DATE_PAT+"(?:\\s+|$))?)+(?<donedate>"+DATE_PAT+"(?:\\s*|$))?)?(?<priority>"+regex_priority.pattern()+")?(?<inputdate>"+DATE_PAT+"(?:\\s*|$))?(.*)");



bool task::is_txt_compatible()
/* Some checks to do if a task can be saved as todo.txt.  usefull?
   #IDEA: can make use of regex_todo_line ?
*/{
	return true;
}

task::task(QString s, QString context, bool loaded)
/* Create a task based on input String. 
	If loaded=true, don't assume any default (task is loaded from file)
	Context is a base info to be added.
*/
{
//	qDebug()<<"task:task Task constructor s="<<s<<"  context="<<context<<"  loaded="<<loaded<<endline;
	static QRegularExpression regex_tuid("\\s+tuid:(?<tuid>.{8}-.{4}-.{4}-.{4}-.{12})");  //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
	static QRegularExpression regex_timetag("\\s+ttag:(?<ttag>\\d{10,15})");

	QString ret=s;
	if (loaded) //we loaded from the file, we should find the data in task (tuid + timetag)
	{
		auto match = regex_tuid.match(s);
		if (match.hasMatch()){
			_tuid= QUuid::fromString(match.captured("tuid"));
			ret.remove(regex_tuid);
		}
		if (_tuid.isNull())
			_tuid=QUuid::createUuid();


		match = regex_timetag.match(s);
		if (match.hasMatch()){
			_ttag= QDateTime::fromSecsSinceEpoch(match.captured("ttag").toLongLong());
			ret.remove(regex_timetag);
		}
		if (!_ttag.isValid())
			_ttag=QDateTime::currentDateTime();

		parse(ret,true);
	}
	else //this is a new typed task, default inputdate if settings + tuid + ttag??
	{
		QSettings settings;	
		
		_tuid=QUuid::createUuid();
		if (!context.isEmpty())
				ret = ret+" "+context;
		
		_ttag=QDateTime::currentDateTime();
//		qDebug()<<" ttag:"+QString::number(_ttag.toSecsSinceEpoch())<<endline;
		parse(ret,true);

    	if(!inputD.isValid()&&(settings.value(SETTINGS_DATES).toBool()))
    		 	setInputDate(QDateTime::currentDateTime());
	
		if (priority.isNull()) // there was no priority in the newly created task
				setPriority(settings.value(SETTINGS_DEFAULT_PRIORITY,DEFAULT_DEFAULT_PRIORITY).toChar());			
		_raw.remove(regex_tuid);
	}

}

task::task(QString s, QUuid tuid)
/* */{
	_raw=s;
	_tuid= tuid;
	_ttag=QDateTime::currentDateTime();
	parse(s,true);

}

task::task(task* copy)
/* Creates a new task with a new TUID
*/{
	setRaw(copy->getRaw());
	//_tuid=copy->getTuid();
	_tuid=QUuid::createUuid();
	_ttag=QDateTime::currentDateTime();
	setInputDate(QDateTime::currentDateTime());
}

task::~task()
/*  nothing to do */
{}

void task::parse(QString s,bool strict)
/* Update the fields because the "text" has changed.  Only tak care of relative dates...
*/
{
	Q_UNUSED(strict)
//	qDebug()<<"task::parse started. s="<<s<<endline;
	QSettings settings;
	_raw=s;
	QString tmp="";

	auto matches = regex_due_date.globalMatch(s);
	while (matches.hasNext())
	{
		tmp=matches.next().captured(1);
		if (tmp.size() > 0)
				dueD = QDateTime::fromString(tmp.remove(0,4),"yyyy-MM-dd");
	}

	matches = regex_due_date_r.globalMatch(s);
	while (matches.hasNext())
	{
		QDateTime td = task::getRelativeDate(matches.next().captured(1));
		if (td.isValid()){ //if new date is not valid, do nothing.
			_raw.remove(regex_due_date_r);
			_raw.append(" due:"+td.toString("yyyy-MM-dd"));
			dueD=td;
		}
	}

	matches = regex_threshold_date.globalMatch(s);
	while (matches.hasNext())
	{
		tmp=matches.next().captured(1);
		if (tmp.size() >0)
			thrD = QDateTime::fromString(tmp.remove(0,2),"yyyy-MM-dd");
	}

	matches = regex_threshold_date_r.globalMatch(s);
	while (matches.hasNext())
	{
//		this->setThresholdDate(matches.next().captured(1),strict);
		QDateTime td = task::getRelativeDate(matches.next().captured(1));
		if (td.isValid()){ //if new date is not valid, do nothing
			_raw.remove(regex_threshold_date_r);
			_raw.append(" t:"+td.toString("yyyy-MM-dd"));
			thrD = td;
		}
	}


	matches = regex_preamble.globalMatch(_raw);
	if (matches.hasNext()){
		auto nn=matches.next();
		if(!nn.captured("px").isNull() | !nn.captured("pxd").isNull())
				this->complete=Qt::Checked;
		else
				this->complete=Qt::Unchecked;
			
		QDateTime d=QDateTime::fromString(nn.captured("pid"),"yyyy-MM-dd");
		if (d.isValid())
			inputD=d;
		else
			inputD=QDateTime();
		
		if (!nn.captured("ppr").isNull())
				priority=nn.captured("ppr").at(1);
		else
			priority = QChar::Null;
	}
	
	QString c;
	matches = regex_color.globalMatch(_raw);
	if (matches.hasNext()){
		c = matches.next().captured(1);
		if (QColor::isValidColorName(c)){
			color=QColor::fromString(c);
		}
	}
	else
		color=QColor::fromString("White");

	_ttag=QDateTime::currentDateTime();

//	qDebug()<<"task::parse "<<toString()<<endline;
}


void task::setDueDate(QDateTime d)
/* Update the due_date
   remove any present dueDate + add due:... at the end.
*/
{
	if(! d.isValid()) return;
	dueD=d;
	_raw.remove(regex_due_date);
	_raw.append(" due:"+d.toString("yyyy-MM-dd"));
	_ttag=QDateTime::currentDateTime();

}

void task::setThresholdDate(QDateTime d)
/* Update the threshold_date
   remove any present thD + add t:... at the end.
*/
{
	if(! d.isValid()) return;
	thrD = d;
	_raw.remove(regex_threshold_date);
	_raw.append(" t:"+d.toString("yyyy-MM-dd"));
	_ttag=QDateTime::currentDateTime();

}

void task::setInputDate(QDateTime d)
/* place a date at beginning, but after priority.
If a date is present, remove it first !*/
{
	QString traw = _raw;
	
	auto matches = regex_preamble.globalMatch(traw);
	if (matches.hasNext())	{
		auto nn=matches.next();
		traw=nn.captured("ptk");
		traw.prepend(d.toString("yyyy-MM-dd")+" ");
		traw.prepend(nn.captured("ppr"));
		traw.prepend(nn.captured("pxd"));
		traw.prepend(nn.captured("px"));
		_raw=traw;
	}
	
	inputD=d;
	_ttag=QDateTime::currentDateTime();

}

void task::setColor(QString c)
/* set / change the color.  if a color is present, it is updated,  if multiple colors are present, they got cleaned */
{
	_raw.remove(regex_color);
	auto matches = regex_color.globalMatch(_raw);
	if (matches.hasNext()){
		c = matches.next().captured(1);
		if (QColor::isValidColorName(c)){
			color=QColor::fromString(c);
		}
	}
	else
		color=QColor::fromString("White");

	_ttag=QDateTime::currentDateTime();

}

void task::setColor(QColor c)
{
	color=c;	
	_ttag=QDateTime::currentDateTime();

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

void task::setPriority(QChar c)
/* Set the priority of task
#BUG: il faut enlever tout ce qui ressemble à une priorité. Si il y en a 2 d'affilée, on enleve les 2.
*/
{
	_raw.remove(regex_priority);	
	if (c.isLetter()){
		_raw.prepend(") ");
		_raw.prepend(c);
		_raw.prepend("(");
		priority=c;
		_ttag=QDateTime::currentDateTime();
	}
}

void task::setRaw(QString s)
/* Set raw text, by first interpreting it.
*/
{
	parse(s);
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
	task *ret=nullptr;
//	_raw.remove(regex_done);
	if (c){
		auto matches = regex_rec.globalMatch(_raw);
		while (matches.hasNext())
		{
			QString tmp = matches.next().captured(1);
			ret = new task(this);
			if (settings.value(SETTINGS_DEFAULT_THRESHOLD,DEFAULT_DEFAULT_THRESHOLD) == "t:"){
				ret->setThresholdDate(task::getRelativeDate(tmp, getThresholdDate()));
				}
			else{
				ret->setDueDate(task::getRelativeDate(tmp, getDueDate()));
				}
			}
    	if(settings.value(SETTINGS_DATES).toBool())
 				_raw.prepend("x "+QDateTime::currentDateTime().toString("yyyy-MM-dd")+" ");
 		else
 				_raw.prepend("x ");
 		complete=Qt::Checked;
	}
	else{
		_raw.remove(regex_completedate);
		complete=Qt::Unchecked;
	}
	_ttag=QDateTime::currentDateTime();

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

QString task::getDisplayText() const
/* text for display in todour*/
{
	QSettings settings;
	QString display = _raw;	
	auto matches = regex_preamble.globalMatch(_raw);
	if (matches.hasNext())
	{
		auto nn=matches.next();
		display=nn.captured("ptk");
		if (settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool())
				display.prepend(nn.captured("pid"));
				
		display.prepend(nn.captured("ppr"));
		
	}
	display.remove(regex_color);
	return display;
}


QString task::getDescription() const
/* returns only the descriptive part of the text, without t: due: color:
*/
{
	qDebug()<<"   task::getDescription not optimised"<<endline;
	QString ret = _raw;
	ret.remove(regex_color);
	ret.remove(regex_done);
	ret.remove(regex_threshold_date);
	ret.remove(regex_due_date);
	ret.remove(regex_rec);
	return ret;
} 

QString task::toSaveString() const
/* returns the full QString for saving, including all hidden data.
*/{
	QString ret=getEditText();
	ret.append(" tuid:"+_tuid.toString(QUuid::WithoutBraces));
	ret.append(" ttag:"+QString::number(_ttag.toSecsSinceEpoch()));
	return ret;
}

QString task::toSaveString_pureTODO() const
/* returns the full QString for saving, including all hidden data.
*/{
	return getEditText();
}

QString task::toString() const
/* returns the full QString for debugging, including all hidden data.
*/
{
	QString ret=_raw;
	ret.append("\\n   tuid:"+_tuid.toString(QUuid::WithoutBraces));
	ret.append("\\n   input Date:"+inputD.toString());
	ret.append("\\n   due Date:"+dueD.toString());
	ret.append("\\n   threshold Date:"+thrD.toString());
	ret.append("\\n   complete Date:"+_complete_date.toString());
	ret.append("\\n   priority:"+QString(priority));
	ret.append("\\n   color:"+color.name());
	ret.append("\\n   complete:"+complete);
	ret.append("\\n   display:"+getDisplayText());
	return ret;
}



bool task::isActive() const
/* returns true if the task is active. Active means:
	- threshold date not greater than today
	- task not completed
	- context threshold not activated (!)
	- not containing "inactive" tags (LATER: IDEA: LOW: WAIT: ..)
	#TODO: remove from here all the QSettings part, to be done upper, will be fast thanks to new _valid model.
*/
{
	QSettings settings;
	bool ret=true;
	ret &= (QDateTime::currentDateTime()>=thrD);

	QStringList words = settings.value(SETTINGS_INACTIVE,DEFAULT_INACTIVE).toString().split(';');
	for (QString i:words){
		ret &= ! _raw.contains(i);
		}
	return ret;
}

/*
====================================  STATIC FUNCTIONS ========================================
*/
QDateTime task::getRelativeDate(QString d, const QDateTime* base)
/* static function, returns a QDateTime object based on the "relative" as described in regex_reldate
*/
{
	QSettings settings;
	QDateTime ret;
	if (base == nullptr)
	    ret = QDateTime::currentDateTime();
	else
		ret = *base;
		
    QRegularExpressionMatch m = regex_reldate.match(d);
//	qDebug()<<"task::getRelativeDate match:"<<m.captured("rdate")<<endline;
    if (!m.hasMatch()) return QDateTime();
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
                if(business_days.contains(ret.date().dayOfWeek())){
                    days++;
                }
            }
        }
        //return d.toString("yyyy-MM-dd")+extra;
        return ret;
}



void task::taskTestSession()
{
//	qDebug()<<"Testing regularExpression : "<<task::testRegularExpressions()<<endline;
 	//test zone

	qDebug()<<"Test zone: "<<endline;
//	task *t = new task ("(B) This is a full test task");
//	qDebug()<<"Created. Let's test:"<<endline;

	QString ttt = "(B) 2025-05-09 test task";
	auto matches = regex_preamble.globalMatch(ttt);
	while (matches.hasNext())
	{
		auto nn=matches.next();
		qDebug()<<"  test m1c0:"<<nn.captured("px")<<endline;
		qDebug()<<"  test m1c1:"<<nn.captured("pxd")<<endline;
		qDebug()<<"  test m1c2:"<<nn.captured("ppr")<<endline;
		qDebug()<<"  test m1c2:"<<nn.captured("pid")<<endline;
		qDebug()<<"  test m1c2:"<<nn.captured("ptk")<<endline;
	
	QString t=nn.captured("ptk");
	t.prepend(nn.captured("ppr"));
	qDebug()<<"original: "<<ttt<<endline;
	qDebug()<<"modified: "<<t<<endline;

	}

	


qDebug()<<"End of test session"<<endline<<endline<<endline<<endline;
}
