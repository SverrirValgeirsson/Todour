#include "todotxt.h"

// Todo.txt file format: https://github.com/ginatrapani/todo.txt-cli/wiki/The-Todo.txt-Format

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDate>
#include <set>
#include <QSettings>
#include <QRegularExpression>
#include <QDebug>
#include "def.h"

todotxt::todotxt()
{
}

void todotxt::setdirectory(QString &dir){
    filedirectory=dir;
}

static QRegularExpression regex_project("\\s(\\+[^\\s]+)");
static QRegularExpression regex_context("\\s(\\@[^\\s]+)");

void todotxt::parse(){
    QSettings settings;
    //qDebug()<<"todotxt::parse";
    // parse the files todo.txt and done.txt (for now only todo.txt)
    todo.clear();
    active_contexts.clear();
    active_projects.clear();
    QString todofile=getTodoFile();
    QFile file(todofile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine();
        todo.push_back(line);
     }

    if(settings.value(SETTINGS_SHOW_ALL,DEFAULT_SHOW_ALL).toBool()){
        // Donefile as well
        QString donefile = getDoneFile();
        QFile file2(donefile);
        if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in2(&file2);
        in.setCodec("UTF-8");
        while (!in2.atEnd()) {
            QString line = in2.readLine();
            todo.push_back(line);
         }
    }


      if(settings.value(SETTINGS_THRESHOLD_LABELS).toBool()){
          // Get all active tags with either a @ or a + sign infront of them, as they can be used for thresholds
          foreach (auto line, todo) {
              if(line.startsWith("x ")){
                  continue; // Inactive so we don't care
              } else {
                auto matches = regex_project.globalMatch(line);
                while(matches.hasNext()){
                    auto match = matches.next();
                    auto project = match.captured(1);
                    active_projects.insert(project);
                }
                matches = regex_context.globalMatch(line);
                while(matches.hasNext()){
                    auto match = matches.next();
                    active_contexts.insert(match.captured(1));
                }
              }

          }
      }
}

QString todotxt::getTodoFile(){
    QSettings settings;
    QString dir = settings.value(SETTINGS_DIRECTORY).toString();
    QString todofile = dir.append(TODOFILE);
    return todofile;
}


QString todotxt::getDoneFile(){
    QSettings settings;
    QString dir = settings.value(SETTINGS_DIRECTORY).toString();
    QString todofile = dir.append(DONEFILE);
    return todofile;
}
void todotxt::getActive(QString& filter,vector<QString> &output){
        // Obsolete... remove?
    Q_UNUSED(filter);
        for(vector<QString>::iterator iter=todo.begin();iter!=todo.end();iter++){
                if( (*iter).length()==0 || (*iter).at(0) == 'x')
                        continue;
                output.push_back( (*iter));
        }
}


bool todotxt::isInactive(QString &text){
    QSettings settings;
    QString t=settings.value(SETTINGS_INACTIVE).toString();
    if(t.isEmpty())
        return false;
    QStringList inactives = t.split(";");
    for(int i=0;i<inactives.count();i++){
        if(text.contains(inactives[i])){
            return true;
        }
    }

    if(settings.value(SETTINGS_THRESHOLD_INACTIVE).toBool()){
        return threshold_hide(text);
    }

    return false;
}

/* Comparator function.. We need to remove all the junk in the beginning of the line */
bool todotxt::lessThan(QString &s1,QString &s2){
    return prettyPrint(s1).toLower() < prettyPrint(s2).toLower();
}

static QRegularExpression regex_threshold_date("t:(\\d\\d\\d\\d-\\d\\d-\\d\\d)");
static QRegularExpression regex_threshold_project("t:(\\+[^\\s]+)");
static QRegularExpression regex_threshold_context("t:(\\@[^\\s]+)");
static QRegularExpression regex_due_date("due:(\\d\\d\\d\\d-\\d\\d-\\d\\d)");

bool todotxt::threshold_hide(QString &t){
    QSettings settings;
    if(settings.value(SETTINGS_THRESHOLD).toBool()){
        auto matches=regex_threshold_date.globalMatch(t);
        while(matches.hasNext()){
            QString today = getToday();
            QString td = matches.next().captured(1);
            if(td.compare(today)>0){
                return true; // Don't show this one since it's in the future
            }

        }
    }


    if(settings.value(SETTINGS_THRESHOLD_LABELS).toBool()){
        auto matches=regex_threshold_project.globalMatch(t);
        while(matches.hasNext()){
            QString project = matches.next().captured(1);
            if(active_projects.count(project)==1)
                return true; // There is an active project with this name, so we skip this
        }

        matches=regex_threshold_context.globalMatch(t);
        while(matches.hasNext()){
            QString project = matches.next().captured(1);
            if(active_contexts.count(project)==1)
                return true; // There is an active project with this name, so we skip this
        }
    }
    return false;
}


void todotxt::getAll(QString& filter,vector<QString> &output){
        // Vectors are probably not the best here...
    Q_UNUSED(filter);
        set<QString> prio;
        vector<QString> open;
        vector<QString> done;
        vector<QString> inactive;
        QSettings settings;
        QString t=settings.value(SETTINGS_INACTIVE).toString();
        QStringList inactives = t.split(";");
        if(!t.contains(";")){
            // There is really nothing here but inactives will still have one item. Lets just remove it
            inactives.clear();
        }

        bool separateinactives = settings.value(SETTINGS_SEPARATE_INACTIVES).toBool();

        for(vector<QString>::iterator iter=todo.begin();iter!=todo.end();iter++){
            QString line = (*iter); // For debugging
            if(line.isEmpty())
                continue;

            // Begin by checking for inactive, as there are two different ways of sorting those
            bool inact=false;
            for(int i=0;i<inactives.count();i++){
                if((*iter).contains(inactives[i])){
                    inact=true;
                    break;

                }
            }

            // If we are respecting thresholds, we should check for that
            bool no_show_threshold = threshold_hide((*iter));


            if(no_show_threshold){
                if(settings.value(SETTINGS_THRESHOLD_INACTIVE).toBool()){
                    inact=true;
                } else {
                    continue;
                }
            }


            if(!(inact&&separateinactives) && (*iter).at(0) == '(' && (*iter).at(2) == ')'){
                prio.insert((*iter));
            } else if ( (*iter).at(0) == 'x'){
                done.push_back((*iter));
            } else if(inact){
                inactive.push_back((*iter));
            }
            else {
                open.push_back((*iter));
            }
        }

        // Sort the open and done sections alphabetically if needed

        if(settings.value(SETTINGS_SORT_ALPHA).toBool()){
            qSort(open.begin(),open.end(),lessThan);
            qSort(inactive.begin(),inactive.end(),lessThan);
            qSort(done.begin(),done.end(),lessThan);
            /*qSort(open);
            qSort(inactive);
            qSort(done);*/
        }

        for(set<QString>::iterator iter=prio.begin();iter!=prio.end();iter++)
            output.push_back((*iter));
        for(vector<QString>::iterator iter=open.begin();iter!=open.end();iter++)
            output.push_back((*iter));
        for(vector<QString>::iterator iter=inactive.begin();iter!=inactive.end();iter++)
            output.push_back((*iter));
        for(vector<QString>::iterator iter=done.begin();iter!=done.end();iter++)
            output.push_back((*iter));
}

Qt::CheckState todotxt::getState(QString& row){
    if(row.length()>1 && row.at(0)=='x' && row.at(1)==' '){
        return Qt::Checked;
    } else {
        return Qt::Unchecked;
    }
}

QString todotxt::getToday(){
    QDate d = QDate::currentDate();
    return d.toString("yyyy-MM-dd");
}

QString todotxt::getRelativeDate(QString shortform,QDate d){
    QString extra = "";
    // The short form supported for now is +\\dd
    QRegularExpression reldateregex("\\+(\\d+)([dwmypb])");
    QRegularExpressionMatch m = reldateregex.match(shortform);
    if(m.hasMatch()){
        if(m.captured(2).contains('d')){
            d= d.addDays(m.captured(1).toInt());
        } else if(m.captured(2).contains('w')){
            d= d.addDays(m.captured(1).toInt()*7);
        } else if(m.captured(2).contains('m')){
            d= d.addMonths(m.captured(1).toInt());
        } else if(m.captured(2).contains('y')) {
            d= d.addYears(m.captured(1).toInt());
        } else if(m.captured(2).contains('p')){
            // Ok. This is the procrastination 'feature'. Add a random number of days and also say that this was procrastrinated
            d = d.addDays(rand()%m.captured(1).toInt()+1);
            extra = " +procrastinated";
        } else if (m.captured(2).contains('b')){
            // Business days. Naive implementation
            // 1=Monday, 6=Sat, 7=sun
            int days=0;
            int addDays = m.captured(1).toInt();
            while(days<addDays){
                d = d.addDays(1); // add one at a time
                if(d.dayOfWeek() <6){
                    days++;
                }
            }

        }
        return d.toString("yyyy-MM-dd")+extra;
    } else {
        return shortform; // Don't know what to do.. so just put the string back
    }
}

QString todotxt::prettyPrint(QString& row){
    QString ret;
    QSettings settings;

    // Remove dates
    todoline tl;
    String2Todo(row,tl);

    ret = tl.priority;
    if(settings.value(SETTINGS_SHOW_DATES).toBool()){
        ret.append(tl.closedDate+tl.createdDate);
    }

    ret.append(tl.text);

    // Remove all leading and trailing spaces
    return ret.trimmed();
}

void todotxt::slurp(QString& filename,vector<QString>& content){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine();
        content.push_back(line);
     }
}

void todotxt::write(QString& filename,vector<QString>&  content){
    //qDebug()<<"todotxt::write("<<filename<<")";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

        QTextStream out(&file);
        out.setCodec("UTF-8");
        for(unsigned int i = 0;	i<content.size(); i++)
            out << content.at(i) << "\n";
}

void todotxt::remove(QString line){
    // Remove the line, but perhaps saving it for later as well..
    QSettings settings;
    if(settings.value(SETTINGS_DELETED_FILE).toBool()){
        QString dir = settings.value(SETTINGS_DIRECTORY).toString();
        QString deletedfile = dir.append(DELETEDFILE);
        vector<QString> deleteddata;
        slurp(deletedfile,deleteddata);
        deleteddata.push_back(line);
        write(deletedfile,deleteddata);
    }
    QString tmp;
    update(line,false,tmp);
}


void todotxt::archive(){
    // Slurp the files
    QSettings settings;
    QString todofile = getTodoFile();
    QString donefile = getDoneFile();
    vector<QString> tododata;
    vector<QString> donedata;
    slurp(todofile,tododata);
    slurp(donefile,donedata);
    for(vector<QString>::iterator iter=tododata.begin();iter!=tododata.end();){
        if((*iter).length()>0 && (*iter).at(0)=='x'){
            donedata.push_back((*iter));
            iter=tododata.erase(iter);
        } else {
            // No change
            iter++;
        }

    }
    write(todofile,tododata);
    write(donefile,donedata);
    parse();
}

void todotxt::refresh(){
    parse();
}

void todotxt::update(QString &row, bool checked, QString &newrow){
    // First slurp the file.
    QSettings settings;
    QString todofile = getTodoFile();
    vector<QString> data;
    slurp(todofile,data);
    QString additional_item = ""; // This is for recurrence. If there is a new item created, put it here since we have to add it after the file is written

    // Preprocessing of the line
    if(settings.value(SETTINGS_THRESHOLD).toBool()){
        QRegularExpression threshold_shorthand("(t:\\+\\d+[dwmypb])");
        QRegularExpressionMatch m = threshold_shorthand.match(newrow);
        if(m.hasMatch()){
            newrow = newrow.replace(m.captured(1),"t:"+getRelativeDate(m.captured(1).mid(2)));
        }        
    }

    if(settings.value(SETTINGS_DUE).toBool()){
        QRegularExpression due_shorthand("(due:\\+\\d+[dwmypb])");
        QRegularExpressionMatch m = due_shorthand.match(newrow);
        if(m.hasMatch()){
            newrow = newrow.replace(m.captured(1),"due:"+getRelativeDate(m.captured(1).mid(2)));
        }
    }

    if(row.isEmpty()){
        todoline tl;
        String2Todo(newrow,tl);
        // Add a date to the line if where doing dates
        if(settings.value(SETTINGS_DATES).toBool()){
            QString today = getToday()+" ";
            tl.createdDate = today;
        }

        // Just add the line
        data.push_back(Todo2String(tl));

    } else {
        for(vector<QString>::iterator iter=data.begin();iter!=data.end();iter++){
            QString *r = &(*iter);
            if(!r->compare(row)){
                // Here it is.. Lets modify if we shouldn't remove it alltogether
                if(newrow.isEmpty()){
                    // Remove it
                    iter=data.erase(iter);
                    break;
                }

                if(checked && !r->startsWith("x ")){
                    todoline tl;
                    String2Todo(*r,tl);
                    tl.checked=true;

                    QString date;
                    if(settings.value(SETTINGS_DATES).toBool()){
                            date.append(getToday()+" "); // Add a date if needed
                    }
                    tl.closedDate=date;

                    // Handle recurrance
                    //QRegularExpression rec_normal("(rec:\\d+[dwmyb])");
                    QRegularExpression rec("(rec:\\+?\\d+[dwmyb])");

                    // Get the "addition" from rec
                    QRegularExpressionMatch m = rec.match(tl.text);
                    if(m.hasMatch()){
                        // Figure out what date we should use
                        bool isStrict = true;
                        QString rec_add = m.captured(1).mid(4);
                        // Add a '+' if it's not there due to how getRelativeDate works
                        if(rec_add.at(0)!= '+'){
                            rec_add.insert(0,'+');
                            isStrict = false;
                        }
                       // Make a copy. It's time to start altering that one

                        if(!tl.priority.isEmpty()){
                            additional_item = tl.priority+tl.text;
                        } else {
                            additional_item = tl.text;
                        }

                        // Get the t:
                        auto mt = regex_threshold_date.globalMatch(tl.text);
                        while(mt.hasNext()){
                            QString old_t = mt.next().captured(1);
                            QString newdate = isStrict?getRelativeDate(rec_add, QDate::fromString(old_t,"yyyy-MM-dd")):getRelativeDate(rec_add);
                            additional_item.replace("t:"+old_t,"t:"+newdate);
                        }
                        // Get the due:
                        auto md = regex_due_date.match(tl.text);
                        if(md.hasMatch()){
                            QString old_due = md.captured(1);
                            QString newdate = isStrict?getRelativeDate(rec_add, QDate::fromString(old_due,"yyyy-MM-dd")):getRelativeDate(rec_add);
                            additional_item.replace("due:"+old_due,"due:"+newdate);
                        }



                    }


                    *r=Todo2String(tl);

                }
                else if(!checked && r->startsWith("x ")){
                    todoline tl;
                    String2Todo(*r,tl);
                    tl.checked=false;
                    tl.closedDate="";
                    *r=Todo2String(tl);
                } else {
                    todoline tl;
                    String2Todo(row,tl);
                    todoline newtl;
                    String2Todo(newrow,newtl);
                    tl.priority=newtl.priority;
                    tl.text=newtl.text;
                    tl.createdDate = newtl.createdDate;
                    tl.closedDate = newtl.closedDate;
                    *r = Todo2String(tl);
                }
                break;
            }
        }
    }

    write(todofile,data);
    if(!additional_item.isEmpty()){
        QString empty="";
        this->update(empty,false,additional_item);
    }
    parse();
}

// A todo.txt line looks like this
static QRegularExpression todo_line("(x\\s+)?(\\([A-Z]\\)\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d\\s+)?(.*)");

void todotxt::String2Todo(QString &line,todoline &t){
    QRegularExpressionMatch match = todo_line.match(line);
    if(match.hasMatch() && match.lastCapturedIndex()==5){

        if(match.captured(1).isEmpty()){
            t.checked=false;
        } else {
            t.checked=true;
        }

        t.priority=match.captured(2);
        if(t.checked){
            t.closedDate=match.captured(3);
            t.createdDate=match.captured(4);
        } else {
            t.createdDate=match.captured(3); // No closed date on a line that isn't closed.
        }
        t.text = match.captured(5);


    } else {
        t.checked=false;
        t.priority="";
        t.closedDate="";
        t.createdDate="";
        t.text="";
    }

}

QString todotxt::Todo2String(todoline &t){
    QString ret;
    QSettings settings;

    // Yep, an ugly side effect, but it make sure we're having the right format all the time
    if(t.checked && t.createdDate.isEmpty()){
        t.createdDate = t.closedDate;
    }

    if(t.checked){
        ret.append("x ");
    } else {
        // Priority shall only be written if we are active
        ret.append(t.priority);
    }
    ret.append(t.closedDate);
    ret.append(t.createdDate);
    // Here we have to decide how to handle priority tag if we have one
    if(t.checked && !t.priority.isEmpty()){
        prio_on_close how = (prio_on_close) settings.value(SETTINGS_PRIO_ON_CLOSE,DEFAULT_PRIO_ON_CLOSE).toInt();
        switch(how){
            case prio_on_close::removeit:
                break; // We do nothing. Just forget it exists
            case prio_on_close::moveit:
                ret.append(t.priority+" "); // Put the priority first in the text
                break;
            case prio_on_close::tagit:
                if(t.priority.size()>2){
                    ret.append("pri:");
                    ret.append(t.priority.at(1));
                    ret.append(" ");
                }
                break;
        }
    }

    ret.append(t.text);
    return ret;
}

// Check when this is due


int todotxt::dueIn(QString &text){
    int ret=INT_MAX;
    QSettings settings;
    if(settings.value(SETTINGS_DUE).toBool()){
        QRegularExpressionMatch m=regex_due_date.match(text);
        if(m.hasMatch()){
            QString ds = m.captured(1);
            QDate d = QDate::fromString(ds,"yyyy-MM-dd");
            return (int) QDate::currentDate().daysTo(d);
        }
    }
    return ret;
}

//QRegularExpression regex_url("[a-zA-Z0-9_]+://[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=\\(\\)]*)");
static QRegularExpression regex_url("[a-zA-Z0-9_]+:\\/\\/([-a-zA-Z0-9@:%_\\+.~#?&\\/=\\(\\)\\{\\}\\\\]*)");

QString todotxt::getURL(QString &line){
    QRegularExpressionMatch m=regex_url.match(line);
    if(m.hasMatch()){
        //qDebug()<<"URL: "<<m.capturedTexts().at(0);
        return m.capturedTexts().at(0);
    }
    else{
        return "";
    }
}
