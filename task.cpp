#include "task.h"
#include "def.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

static QRegularExpression regex_todo_line("(x\\s+)?(\\([A-Z]\\)\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(.*)");
static QRegularExpression regex_tuid("tuid:(.\\{8\\}-.\\{4\\}-.\\{4\\}-.\\{4\\}-.\\{12\\})");  //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
static QRegularExpression regex_date("\\d\\d\\d\\d-\\d\\d-\\d\\d");
static QRegularExpression regex_reldate("\\+(\\d+)([dwmypb])");
static QRegularExpression regex_url("[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");
static QRegularExpression regex_color("color:([a-z]*)");
static QRegularExpression regex_priority("^\\((\\w)\\) +");
static QRegularExpression regex_done("^(x )*");
static QRegularExpression regex_threshold_project("t:(\\+[^\\s]+)(?: |$)");
static QRegularExpression regex_threshold_context("t:(\\@[^\\s]+)(?: |$)");

static QRegularExpression regex_threshold_date("t:((" + regex_date.pattern() + "|" + regex_reldate.pattern()+"))(?: |$)");
static QRegularExpression regex_due_date("due:((" + regex_date.pattern() + "|" + regex_reldate.pattern()+"))(?: |$)");
static QRegularExpression regex_rec("(rec:" + regex_reldate.pattern()+")(?: |$)"); 

bool task::is_txt_compatible()
/* Some checks to do if a task can be saved as todo.txt.  usefull?
*/
{ return true;
}

task::task(QString s)
/*
*/
{
	// do some checks
	// generate tuid
	// save the _raw	
	// generate all the internal variables. to be externalised
	update(s.replace('\n',' '),true);

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
	_d_date = QDate();
	_t_date = QDate();

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
	if (match.isValid()){
		this->set_color(match.captured(1));
	}
	
	_priority = "";
	match = regex_priority.match(s);
	if (match.isValid()){
		this->set_priority(match.captured(1));
		}
		
	if (!s.isEmpty()) this->set_complete(s.at(0)=='x');
		
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
		if (!_d_date.isValid() || td<=_d_date || strict){
			_raw.remove(regex_due_date);
			_d_date=td;
			_raw.append(" due:"+td.toString("yyyy-MM-dd"));
			
		}		
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
		if (!_t_date.isValid() || td>=_t_date || strict){
			_raw.remove(regex_threshold_date);
			_t_date=td;
			_raw.append(" t:"+td.toString("yyyy-MM-dd"));
		}
	}
}


void task::set_color(QColor c)
/* Fonction used to set / change the color.
 if a color is present, it is updated.
 if multiple colors are present, they got cleaned
 if no color is present, it is added at the end
 */
{
	_raw.remove(regex_color);
	if (c.isValid())
	{
		_color=c;
		_raw.append(" color:"+c.name());
	}
}

void task::set_color(QString c)
/* Fonction used to set / change the color.
 if a color is present, it is updated.
 if multiple colors are present, they got cleaned
 if no color is present, it is added at the end
 */
{
//#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	if (QColor::isValidColorName(c))
	{
		_raw.remove(regex_color);
		_color=QColor::fromString(c);	
//#else
//	if (QColor::isValidColorName(c))
//	{
//		_raw.remove(regex_color);
		//delete &_color;
//		_color=QColor::fromString(c);
//#endif
		_raw.append(" color:"+c);
	}

}


void task::set_description(QString s)
/*
NOT WORKING IN THIS STATE
*/
{
	// a définir : que contient exactement le text?
	// ceci sera sans doute la fonction la plus utilisée: on a modifié le texte de la tache.
  	//	==> Il faut tout ré-interpréter !
    Q_UNUSED(s);
}


void task::set_priority(QString c)
/*

BUG: il faut enlever tout ce qui ressemble à une priorité. Si il y en a 2 d'affilée, on enleve les 2.
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
		_raw.prepend("x ");
		_completed = true;
	}else{
		_completed = false;
	}
	return ret;
}

QString task::get_display_text() const
/* returns text for display in todour, =_raw - color - leading_dates - leading x
*/
{
	QString ret = _raw;
	ret.remove(regex_color);
	ret.remove(regex_done);
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

QDate task::getRelativeDate(QString d)
/* static function, returns a QDate object based on the "relative" as described in regex_reldate
*/
{
	QSettings settings;
    QDate ret = QDate::currentDate();
    QRegularExpressionMatch m = regex_reldate.match(d);
    if (!m.hasMatch()) return QDate();
        if(m.captured(2).contains('d')){
            ret = ret.addDays(m.captured(1).toInt());
        } else if(m.captured(2).contains('w')){
            ret = ret.addDays(m.captured(1).toInt()*7);
        } else if(m.captured(2).contains('m')){
            ret = ret.addMonths(m.captured(1).toInt());
        } else if(m.captured(2).contains('y')) {
            ret = ret.addYears(m.captured(1).toInt());
        } else if(m.captured(2).contains('p')){
            // Ok. This is the procrastination 'feature'. Add a random number of days and also say that this was procrastrinated
            ret = ret.addDays(rand()%m.captured(1).toInt()+1);
            //extra = " +procrastinated";
        } else if (m.captured(2).contains('b')){
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
            int add = m.captured(1).toInt();
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
