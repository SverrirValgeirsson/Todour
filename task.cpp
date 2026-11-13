#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>

#define DATE_PAT QString("\\d\\d\\d\\d-\\d\\d-\\d\\d")
#define RDATE_PAT QString("\\+(?<n>\\d+)(?<u>[dwmypb])")

static QRegularExpression regex_date("("+DATE_PAT+")(?:\\s*|$)");
static QRegularExpression regex_reldate("(?<rdate>"+RDATE_PAT+")(?:\\s*|$)");

static QRegularExpression regex_url("(?:\\s+)[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_color("(?:\\s+)color:([a-z]*)");
//static QRegularExpression regex_threshold_project("(?:\\s+)t:(\\+[^\\s]+)(?#\\s+|$)");
static QRegularExpression regex_threshold_date("(?:\\s+)(t:" + DATE_PAT + ")(?#\\s+|$)");
static QRegularExpression regex_threshold_date_r("(?:\\s+)(t:" + RDATE_PAT + ")(?#\\s+|$)");

static QRegularExpression     regex_due_date("(?:\\s+)(due:" + DATE_PAT + ")(?#\\s+|$)");
static QRegularExpression     regex_due_date_r("(?:\\s+)(due:" + RDATE_PAT + ")(?#\\s+|$)");

static QRegularExpression regex_rec("(?:\\s+)(rec:" + RDATE_PAT+")(?#\\s+|$)"); 

//preamble    //^(?:x\s+(?:\d\d\d\d-\d\d-\d\d\s)?)?(?:\(\w\)\s+)?(?:\d\d\d\d-\d\d-\d\d\s+)?
static QRegularExpression regex_preamble("^(?<pxd>x\\s+(?:"+DATE_PAT+"\\s)?)?\\s*(?<ppr>[(][a-z,A-Z][)]\\s)?\\s*(?<pid>"+DATE_PAT+"\\s)?\\s*(?<ptk>.*)$");
static QRegularExpression regex_done        ("^(x\\s+)"); // x 2025-03-01
static QRegularExpression regex_completedate("^(x\\s+("+DATE_PAT+")?\\s+)");
static QRegularExpression regex_priority("^(?:x\\s+(?="+DATE_PAT+")?)?(\\(\\w\\)\\s+)"); //(B) 2025-05-05
static QRegularExpression regex_inputdate("^((?:x\\s)|(?:x\\s+"+DATE_PAT+"\\s))?\\s*(?:[(][a-z,A-Z][)]\\s)?\\s*(?<p>"+DATE_PAT+")");
//^((?:x\s)|(?:x\s+\d\d\d\d-\d\d-\d\d\s))?\s*(?:[(][A-Z,a-z][)]\s)?\s*(?<idate>\d\d\d\d-\d\d-\d\d)

static QRegularExpression regex_todo_line("((^(?:x )+("+DATE_PAT+"(?:\\s+|$))?)+(?<donedate>"+DATE_PAT+"(?:\\s*|$))?)?(?<priority>"+regex_priority.pattern()+")?(?<inputdate>"+DATE_PAT+"(?:\\s*|$))?(.*)");

static QRegularExpression regex_progress("\\s+(?:\\#(?<p>\\d\\d)\\%)(?#\\s+|$)");



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
		if (priority.isNull()){ // there was no priority in the newly created task
			QString newp = settings.value(SETTINGS_DEFAULT_PRIORITY,DEFAULT_DEFAULT_PRIORITY).toString();
//			qDebug()<<"Task constructor newp="<<newp<<endline;
			if (newp.size()>0) setPriority(newp.at(0));
			}
		_raw.remove(regex_tuid);
	}
	//default active:
	active=true;
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
	this->active = copy->isActive();
	setInputDate(QDateTime::currentDateTime());
}

/*task::task(const task& in)

{
_raw = in._raw;
	displayText=in.displayText;
//	QString description;
//	QUuid _tuid;
//	QDateTime _ttag;
//	QDateTime _complete_date;
//	QChar priority;
//	QColor color;
//	QDateTime inputD;
//	QDateTime dueD;
//	QDateTime thrD;
//	Qt::CheckState complete ;
//	bool active;
//	QStringList contexts;
//	QStringList thr_contexts;
//	int progress;
}*/

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
//	qDebug()<<"task::parse step 1"<<endline;

	matches = regex_due_date_r.globalMatch(s);
	while (matches.hasNext())
	{
		QDateTime td = task::getRelativeDate(matches.next().captured(1),QDateTime());
		if (td.isValid()){ //if new date is not valid, do nothing.
			_raw.remove(regex_due_date_r);
			_raw.append(" due:"+td.toString("yyyy-MM-dd"));
			dueD=td;
		}
	}
//	qDebug()<<"task::parse step 2"<<endline;
	matches = regex_threshold_date.globalMatch(s);
	while (matches.hasNext())
	{
		tmp=matches.next().captured(1);
		if (tmp.size() >0)
			thrD = QDateTime::fromString(tmp.remove(0,2),"yyyy-MM-dd");
	}
	
//	qDebug()<<"task::parse step 3"<<endline;
	matches = regex_threshold_date_r.globalMatch(s);
	while (matches.hasNext())
	{
//		this->setThresholdDate(matches.next().captured(1),strict);
		QDateTime td = task::getRelativeDate(matches.next().captured(1),QDateTime());
		if (td.isValid()){ //if new date is not valid, do nothing
			_raw.remove(regex_threshold_date_r);
			_raw.append(" t:"+td.toString("yyyy-MM-dd"));
			thrD = td;
		}
	}

//	qDebug()<<"task::parse step 4"<<endline;
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

//	qDebug()<<"task::parse step 5"<<endline;
	matches = regex_inputdate.globalMatch(_raw);
	if (matches.hasNext()){
		auto nnn=matches.next();

		QDateTime d=QDateTime::fromString(nnn.captured("p"),"yyyy-MM-dd");
		if (d.isValid())
			inputD=d;
		else
			inputD=QDateTime();
	}

//	qDebug()<<"task::parse step 6"<<endline;
	matches = regex_progress.globalMatch(_raw);
	if (matches.hasNext()){
		auto nnn=matches.next();
		progress = nnn.captured("p").toInt();
	}
	else progress = 0;

	
	QString c;
//	qDebug()<<"task::parse step 7"<<endline;

	matches = regex_color.globalMatch(_raw);
	if (matches.hasNext()){
		c = matches.next().captured(1);
		if (QColor::isValidColorName(c)){
			color=QColor::fromString(c);
		}
	}
	else
		color=QColor::fromString("White");

//	qDebug()<<"task::parse step 8"<<endline;

	this->updateDisplayText();
//		qDebug()<<"task::parse step 9"<<endline;

	this->updateDescription();
//	qDebug()<<"task::parse step 10"<<endline;

	this->updateContexts();
//	qDebug()<<"task::parse step 11"<<endline;
	
	_ttag=QDateTime::currentDateTime();
//	qDebug()<<"task::parse Task parsed: "<<this->toString()<<endline;
}

void task::updateDisplayText()
/* Update the DisplayText based on _raw.
*/{
	QSettings settings;
	displayText = _raw;	
	auto matches = regex_preamble.globalMatch(_raw);
	if (matches.hasNext())
	{
		auto nn=matches.next();
		displayText=nn.captured("ptk");
		if (settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool())
				displayText.prepend(nn.captured("pid"));
				
		displayText.prepend(nn.captured("ppr"));
		
	}
	displayText.remove(regex_color);

}

void task::updateDescription()
/* Update the Description based on _raw.
*/{
	description = displayText;
	return; //not used, earn some instructions.
	description.remove(regex_threshold_date);
	description.remove(regex_due_date);
	description.remove(regex_rec); 
}

void task::updateContexts()
/*
*/{
//	qDebug()<<"task::updateContexts step 1"<<endline;

	contexts.clear();
	thr_contexts.clear();

	if (complete == Qt::Checked)	return;
//	qDebug()<<"task::updateContexts step 2"<<endline;
 	
 	QRegularExpression regex_context("(?:\\s+|^)((\\@[^\\s]+)|(\\+[^\\s]+))(?#\\s+|$)");
	QRegularExpressionMatchIterator matcher = regex_context.globalMatch(_raw);
	while (matcher.hasNext()) {
 	   QString cont = matcher.next().captured(1);
 	   if (!cont.isNull() && !cont.isEmpty())
		 	   contexts << cont; //we don't care about duplicates...
 	   }
//	qDebug()<<"task::updateContexts step 2"<<endline;

	QRegularExpression regex_threshold_context("(?:\\s+)t:((\\@[^\\s]+)|(\\+[^\\s]+))(?#\\s+|$)");  
	matcher = regex_threshold_context.globalMatch(_raw);
	while (matcher.hasNext()) {
		QString cont = matcher.next().captured(1);
 	   if (!cont.isNull() && !cont.isEmpty())
 	   		thr_contexts << cont; //we don't care about duplicates...
	}

// 	qDebug()<<"task::updateContexts. raw<"<<_raw<<endline;
// 	qDebug()<<"        contexts= "<<contexts<<endline;
//		qDebug()<<"        thr_contexts= "<<thr_contexts<<endline;

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

	this->updateDisplayText();
//	this->updateDescription();
//	this->updateContexts();
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

	this->updateDisplayText();
	this->updateDescription();
	this->updateContexts();
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

	this->updateDisplayText();
//	this->updateDescription();
//	this->updateContexts();
	_ttag=QDateTime::currentDateTime();
}

void task::setProgress(int _prog)
/*
*/{
	progress = _prog;
	_raw.remove(regex_progress);
	if (_prog != 0){
		_raw.append(" #");
		_raw.append(QString::number(_prog));
		_raw.append("%");
		}
	this->updateDisplayText();
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
			_raw.append(" color:");
			_raw.append(c);
		}
	}
	else
		color=QColor::fromString("White");

	this->updateDisplayText();

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
    Q_UNUSED(s);
}

void task::setPriority(QChar c)
/* Set the priority of task
#BUG: il faut enlever tout ce qui ressemble à une priorité. Si il y en a 2 d'affilée, on enleve les 2.
*/
{
	if (c.isLetter()){
		_raw.remove(regex_priority);	
		_raw.prepend(") ");
		_raw.prepend(c);
		_raw.prepend("(");
		priority=c;
		_ttag=QDateTime::currentDateTime();
		this->updateDisplayText();
		this->updateDescription();
//		this->updateContexts();
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
		auto match = regex_rec.match(_raw);
		if (match.hasMatch()){
			QString tmp = match.captured(1);
			ret = new task(this);
			QDateTime new_date;
			if (this->getThresholdDate().isValid()){ // t: is valid, and due is not
				ret->setThresholdDate(task::getRelativeDate(tmp, getThresholdDate()));
				}
			if (this->getDueDate().isValid()){ // due: is valid, and t: is not
				ret->setDueDate(task::getRelativeDate(tmp, getDueDate()));
				}
			if (!this->getThresholdDate().isValid() && !this->getDueDate().isValid()){			  // nor t: nor due: are valid
				if (settings.value(SETTINGS_DEFAULT_THRESHOLD,DEFAULT_DEFAULT_THRESHOLD) == "t:"){
					ret->setThresholdDate(task::getRelativeDate(tmp, QDateTime()));
				}
			else{
				ret->setDueDate(task::getRelativeDate(tmp, QDateTime()));
				}
					
			}

		}
    	if(settings.value(SETTINGS_DATES).toBool())
 				_raw.prepend("x "+QDateTime::currentDateTime().toString("yyyy-MM-dd")+" "); //we keep the priority??
 		else
 				_raw.prepend("x ");
 		complete=Qt::Checked;
	}
	else{
		_raw.remove(regex_completedate);
		complete=Qt::Unchecked;
	}
	this->updateDisplayText();
	this->updateDescription();
	this->updateContexts();
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

void task::recalculateTask(QStringList inactiveflags)
/* determine if the task is active / inactive based on inactiveflags list of keywords
*/{
	active=true;
	for (QString i:inactiveflags){
		if (displayText.contains(i)){
			active=false;
			break;
			}
		}
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

/*
====================================  STATIC FUNCTIONS ========================================
*/
QDateTime task::getRelativeDate(QString d, const QDateTime base)
/* static function, returns a QDateTime object based on the "relative" and today as described in regex_reldate
*/
{
	QSettings settings;
	QDateTime ret;
	if (!base.isValid())
	    ret = QDateTime::currentDateTime();
	else
		ret = base;
		
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
