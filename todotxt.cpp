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

todotxt::todotxt()
{
}

void todotxt::setdirectory(QString &dir){
    filedirectory=dir;
}

void todotxt::parse(){
    //qDebug()<<"todotxt::parse";
    // parse the files todo.txt and done.txt (for now only todo.txt)
    todo.clear();
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

}

QString todotxt::getTodoFile(){
    QSettings settings;
    QString dir = settings.value("directory").toString();
    QString todofile = dir.append(TODOFILE);
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
    QString t=settings.value("inactive").toString();
    if(t.isEmpty())
        return false;
    QStringList inactives = t.split(";");
    for(int i=0;i<inactives.count();i++){
        if(text.contains(inactives[i])){
            return true;
        }
    }
    return false;
}

/* Comparator function.. We need to remove all the junk in the beginning of the line */
bool todotxt::lessThan(QString &s1,QString &s2){
    return prettyPrint(s1).toLower() < prettyPrint(s2).toLower();
}

void todotxt::getAll(QString& filter,vector<QString> &output){
        // Vectors are probably not the best here...
    Q_UNUSED(filter);
        set<QString> prio;
        vector<QString> open;
        vector<QString> done;
        vector<QString> inactive;
        QSettings settings;
        QString t=settings.value("inactive").toString();
        QStringList inactives = t.split(";");
        if(!t.contains(";")){
            // There is really nothing here but inactives will still have one item. Lets just remove it
            inactives.clear();
        }

        bool separateinactives = settings.value("separateinactive").toBool();

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

        if(settings.value("sort_alpha").toBool()){
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

QString todotxt::prettyPrint(QString& row){
    QString ret=row;

    if(ret.length()>1 && ret.at(0) == 'x' && ret.at(1) == ' '){
        ret.remove(0,1);
    }


    // Remove dates
    QStringList l = ret.split(" ",QString::SkipEmptyParts);
    int lastdate =1; // The last part that could be a date
    if(l.length()>=2){
        for(int i=0;i<l.length();i++){

            // The first item can be a prio
            if(l.at(i).at(0)=='(' && l.at(i).at(2)==')'){
               if(i==0){
                    lastdate = 2; // The last date increases by one as there was a prio
                   continue; // Ok Just leave this
               } else {
                   break; // If it's not the first one then we're not removing anything more as the format is wrong
               }
            }

            QDate d = QDate::fromString(l.at(i),"yyyy-MM-dd");
            if(d.isValid()){
                // This is a date. If it's at 0,1 or 2 then we remove it
                if(i<=lastdate){
                    ret.replace(ret.indexOf(l.at(i)),l.at(i).size()+1,"");
                    //ret.replace(l.at(i),"");
                }
            }
        }

    }

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


void todotxt::archive(){
    // Slurp the files
    QSettings settings;
    QString dir = settings.value("directory").toString();
    QString todofile = dir.append(TODOFILE);
    dir = settings.value("directory").toString();
    QString donefile = dir.append(DONEFILE);
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
    QString dir = settings.value("directory").toString();
    QString todofile = dir.append(TODOFILE);
    vector<QString> data;
    slurp(todofile,data);
    if(row.isEmpty()){

        // Add a date to the line if where doing dates
        if(settings.value("dates").toBool()){
            QString today = getToday();
            // The date should be after any ")" or as the first part of the line
            if(newrow.at(0)=='(' && newrow.at(2)==')'){
                QChar prio = newrow.at(1);
                QString rest = newrow.mid(3);
                newrow = '('+prio+") "+today+rest;
            }
            else {
                newrow = today+" "+newrow;
            }
        }
        // Just add the line
        data.push_back(newrow);

    } else {

        //Below are kept for reference. Should never be needed since we can always search for exact mathces
        //QString reg = "(x\\s)?(\\d{4}-\\d{2}-\\d{2}){0,2}"+QRegExp::escape(row)+"$";
        //QRegExp rowregex(reg);

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
                    // Here we need to handle dates

                    /* This is the 2.01 version where I wrongly assumed the date should come after the prio
                    QString prioanddate; // This will be prepended no matter what

                    if(r->at(0)=='('&& r->at(2)==')'){
                        prioanddate="("+QString(r->at(1))+") ";
                        r->replace(prioanddate,""); // Remove the prio from the string for now
                    }
                    else{
                        prioanddate="";
                    }
                    if(settings.value("dates").toBool()){
                        prioanddate.append(getToday()+" ");
                    }
                    r->prepend("x "+prioanddate); */

                    QString date;
                    if(settings.value("dates").toBool()){
                            date.append(getToday()+" "); // Add a date if needed
                    }
                    r->prepend("x "+date);

                }
                if(!checked && r->startsWith("x ")){
                    // Here we must remove a date if there are two
                    // 2.01 erronious behaviour QRegularExpression re("x\\s+(\\([A-Z]\\)\\s+)?(\\d\\d\\d\\d-\\d\\d-\\d\\d )\\d\\d\\d\\d-\\d\\d-\\d\\d ");
                    QRegularExpression re("x\\s+(\\d\\d\\d\\d-\\d\\d-\\d\\d )(\\([A-Z]\\)\\s+)?\\d\\d\\d\\d-\\d\\d-\\d\\d ");

                    QRegularExpressionMatch match = re.match(*r);
                    if(match.hasMatch()){
                        // Cool. There was a match. We need to remove one date
                        //qDebug()<<"Match 1: "<<match.captured(1); // Detta ska vara prion eller tomt i 2.01 men i ny version ska denna vara datumet som ska bort
                        //qDebug()<<"Match 2: "<<match.captured(2); // Detta är datumet som vi får ta bort
                        QString repl = match.captured(1);
                        r->replace(r->indexOf(repl),repl.size(),""); // r->replace(match.captured(2),""); would replace all occurences
                    }
                    QString exet("x ");
                    r->replace(0,exet.size(),"");
                }
                r->replace(row,newrow);
                break;
            }
        }
    }

    write(todofile,data);
    parse();

}
