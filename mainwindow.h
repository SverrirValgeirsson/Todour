#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <uglobalhotkeys.h>
#include <QSystemTrayIcon>
#include <QMenu>

#include <memory>
#include "version.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void parse_todotxt();
    void addTodo(QString &s);
    ~MainWindow();
    
public slots:
    void fileModified(const QString& str);
    void undo();
    void redo();
//    void on_new_version();


private slots:
    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

//    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void dataInModelChanged(QModelIndex i1,QModelIndex i2);

    void on_btn_Alphabetical_toggled(bool checked);

    void on_lineEdit_2_returnPressed();

    void on_hotkey();

    void on_context_lock_toggled(bool checked);


    void on_cb_threshold_inactive_stateChanged(int arg1);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cleanup(); // Need to have a quit slot of my own to save settings and so on.


    void on_pb_closeVersionBar_clicked();

    void on_actionCheck_for_updates_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_actionQuit_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionShow_All_changed();

    void on_actionStay_On_Top_changed();

    void on_actionManual_triggered();
    
    void on_actionPrint_triggered();

    
//Gaetandc 4/1/24
   void on_actionEdit();
   void on_actionDelete();
   void on_actionPostpone();
   void on_actionDuplicate();
   void on_actionPriorityA();
   void on_actionPriorityB();
   void on_actionPriorityC();
   void on_actionPriorityD();
//   void on_actionPrint();
   
   void new_version(QString);


private:
//	QSettings settings;

    void setFileWatch();
    void setTray();
    void clearFileWatch();
    void closeEvent(QCloseEvent *ev);
    Ui::MainWindow *ui;
    void saveTableSelection();
    void resetTableSelection();
    void updateSearchResults();
    void updateTitle();
    void setFontSize();
    void stayOnTop();
    QString baseTitle;
    UGlobalHotkeys *hotkey;
    void setHotkey();
    QSystemTrayIcon *trayicon = NULL;
    QMenu *traymenu=NULL;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    
    QModelIndex currentIndex;
	todour_version *Version;

    
//Gaetandc 4/1/24    
    QMenu* rClickMenu=NULL;
    QMenu* priorityMenu=NULL;

    QAction* editAction;
    QAction* deleteAction;
    QAction* postponeAction;
    QAction* duplicateAction;
    QAction* ApriorAction;
    QAction* BpriorAction;
    QAction* CpriorAction;
    QAction* DpriorAction;
    QAction* actionPrint;
    
};

#endif // MAINWINDOW_H
