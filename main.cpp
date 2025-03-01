#include <QApplication>

//debug:
#include <QDebug>
#include "def.h"

#include "task.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	qDebug()<<"Hello, debug mode."<<endline;
	//task::taskTestSession();

    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon(":/icons/todour.png"));
    w.show();
 

 
    
    return a.exec();
    

}
