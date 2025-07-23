#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    a.setWindowIcon(QIcon(":/icons/todour.png"));
    w.show();
    
    return a.exec();
}
