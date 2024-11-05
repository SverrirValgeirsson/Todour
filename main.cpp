#include <QApplication>

//debug:
#include <QDebug>
#include "def.h"


//testing : task.h / task.cpp
#include "task.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{

 //test zone
/*
qDebug()<<"Test zone: "<<endline;
task *t = new task ("(B) This is a full test task t:2024-10-09 due:2024-11-01 due:2026-01-01 color:red");
task *u = new task ("This is a second test task t:2025-10-09");
task *v = new task ("x This is a finished test task t:2022-10-01");
qDebug()<<"Created. Let's test:"<<endline;

qDebug()<<"t raw: "<<t->get_raw()<<endline;
qDebug()<<"t due-date: "<<t->get_due_date()<<endline;
qDebug()<<"t threshold-date: "<<t->get_threshold_date()<<endline;
qDebug()<<"t text: "<<t->get_display_text()<<endline;
qDebug()<<"t pure text: "<<t->get_pure_text()<<endline;
qDebug()<<"t priority: "<<t->get_priority()<<endline;
qDebug()<<"t color: "<<t->get_color()<<endline;
qDebug()<<"t is Complete: "<<t->isComplete()<<endline;
qDebug()<<"t is Active: "<<t->isActive()<<endline;
qDebug()<<"t utid: "<<t->get_utid()<<endline;


t->set_priority("A");
qDebug()<<"t raw: "<<t->get_raw()<<endline;
t->set_priority("Z");
qDebug()<<"t raw: "<<t->get_raw()<<endline;


/*


qDebug()<<"u raw: "<<u->get_raw()<<endline;
qDebug()<<"u due-date: "<<u->get_due_date()<<endline;
qDebug()<<"u threshold-date: "<<u->get_threshold_date()<<endline;
qDebug()<<"u text: "<<u->get_display_text()<<endline;
qDebug()<<"u pure text: "<<u->get_pure_text()<<endline;
qDebug()<<"u priority: "<<u->get_priority()<<endline;
qDebug()<<"u color: "<<u->get_color()<<endline;
qDebug()<<"u is Complete: "<<u->isComplete()<<endline;
qDebug()<<"u is Active: "<<u->isActive()<<endline;
qDebug()<<"u utid: "<<u->get_utid()<<endline;

qDebug()<<"v raw: "<<v->get_raw()<<endline;
qDebug()<<"v due-date: "<<v->get_due_date()<<endline;
qDebug()<<"v threshold-date: "<<v->get_threshold_date()<<endline;
qDebug()<<"v text: "<<v->get_display_text()<<endline;
qDebug()<<"v pure text: "<<v->get_pure_text()<<endline;
qDebug()<<"v priority: "<<v->get_priority()<<endline;
qDebug()<<"v color: "<<v->get_color()<<endline;
qDebug()<<"v is Complete: "<<v->isComplete()<<endline;
qDebug()<<"v is Active: "<<v->isActive()<<endline;
qDebug()<<"v utid: "<<v->get_utid()<<endline;

qDebug()<<"End of test session"<<endline;
*/


    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon(":/icons/todour.png"));
    w.show();
 

 
    
    return a.exec();
    

}
