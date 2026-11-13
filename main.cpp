#include <QApplication>
#include <iostream>
//debug:
#include <QDebug>
#include <QDir>
#include "def.h"
#include "task.h"
#include "mainwindow.h"
#include "taskset.h"
#include "quickadddialog.h"

void testTasks();

int main(int argc, char *argv[])
{ 	
    QApplication appl(argc, argv);
    if(QCoreApplication::arguments().contains("--help")){
        cout<<"Todour is a personnal offline task management system based on todo.txt"<<endl;
        cout<<" --help : show this help screen"<<endl;
	     cout<<" --portable / -portable : use a local (in folder) config file"<<endl;
	     cout<<" --quickAdd / -a : open task quickadd dialog"<<endl;
	     cout<<" --testRun : run a test sequence"<<endl;
        return 0;
    }
    
    if(QCoreApplication::arguments().contains("--testRun")){
    		testTasks();
    		return -1;
    		}
   
    if(QCoreApplication::arguments().contains("-portable") || QCoreApplication::arguments().contains("--portable")){
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,QDir::currentPath());
        qDebug()<<"Setting ini file path to: "<<QDir::currentPath()<<endline;
    }



    if(QCoreApplication::arguments().contains("--quickAdd") | QCoreApplication::arguments().contains("-a")){
    	// add a always working function. If todour is launched with -a flag, only show the "taskadd" and close.
  		taskset* task_set = new taskset(NULL);
      auto dlg = new QuickAddDialog();
      dlg->setModal(true);
      dlg->show();
      dlg->exec();
      if(dlg->accepted){
      	task_set->addTask(new task(dlg->text));
      }
      task_set->flush();
    	exit(0);
    }

    MainWindow w;
    appl.setWindowIcon(QIcon(":/icons/todour.png"));
    w.show();
    return appl.exec();
    

}





void testTasks()
/*
	task subsystem testing procedure. 
	Create some tasks based on text
	For all tasks, display all the data
*/{

   std::vector<task*> content;

content.push_back(new task("2025-06-22 This is a simple task #12%"));
content.push_back(new task("(A) 2025-06-22 This is a simple task"));
content.push_back(new task("(B) 2025-06-23 This is another task t:2026-03-20"));
content.push_back(new task("x (A) 2025-06-24 This is a simple task due:2030-03-01"));
content.push_back(new task("(A) 2025-06-25 This is a simple task #67% t:2020-01-01 rec:+1y"));
content.push_back(new task("(A) 2025-06-26 This is a simple task t:2026-04-01 due:2026-04-15"));
content.push_back(new task("(A) 2025-06-27 This is a simple task #01% t:+home"));
content.push_back(new task("(A) 2025-06-28 This is a complex task +home #IT @testing"));
content.push_back(new task("(A) 2025-06-29 color:red This is a simple task"));

for (std::vector<task*>::iterator i=content.begin(); i!=content.end();i++){
	qDebug()<<(*i)->getRaw()<<endline;
	qDebug()<<"  Tuid: "<<(*i)->getTuid().toString()<<endline;
	qDebug()<<"  Displaytext: "<<(*i)->getDisplayText()<<endline;
	qDebug()<<"  EditText: "<<(*i)->getEditText()<<endline;
	qDebug()<<"  Description: "<<(*i)->getDescription()<<endline;
	qDebug()<<"  Threshold date: "<<(*i)->getThresholdDate().toString("ddMMMyyyy")<<endline;
	qDebug()<<"  Due date: "<<(*i)->getDueDate().toString("ddMMMyyyy")<<endline;
	qDebug()<<"  Input date: "<<(*i)->getInputDate().toString("ddMMMyyyy")<<endline;
	qDebug()<<"  TimeStamp: "<<(*i)->getTimeStamp().toString("ddMMMyyyy")<<endline;
	qDebug()<<"  Priority: "<<(*i)->getPriority()<<endline;
	qDebug()<<"  Color: "<<(*i)->getColor()->name()<<endline;
	if ((*i)->isComplete() == Qt::Checked) qDebug()<<"  is complete"<<endline;
	else  qDebug()<<"  is not complete"<<endline;

		
	qDebug()<<"  Contexts: "<<(*i)->getContexts()<<endline;		
	qDebug()<<"  Threshold Contexts: "<<(*i)->getThresholdContexts()<<endline;
	qDebug()<<"  URL: "<<(*i)->getURL()<<endline;		
	qDebug()<<"  Progress: "<<(*i)->getProgress()<<endline;
	
	}
	

}
