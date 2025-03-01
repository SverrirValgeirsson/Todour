#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <uglobalhotkeys.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer> //used for hiding version bar
#include <QSortFilterProxyModel>
#include <QDesktopServices> //used for showing the online user manual.

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
    void addTodo(QString &s, QString context);
    ~MainWindow();
    
public slots:
    void undo();
    void redo();


private slots:
    void on_lineEditFilter_textEdited(const QString &arg1);
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_addButton_clicked();
    inline void on_lineEditNew_returnPressed()
    		{on_addButton_clicked();}
    void on_archiveButton_clicked();
    void on_refreshButton_clicked();
    void on_lineEditFilter_returnPressed();
    void on_hotkey();
    void on_context_lock_toggled(bool checked);


    void on_cb_threshold_inactive_stateChanged(int arg1);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cleanup(); // Need to have a quit slot of my own to save settings and so on.

    void on_pb_closeVersionBar_clicked();
    inline void on_actionCheck_for_updates_triggered()
    		{Version->onlineCheck(true);}
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    inline void on_actionQuit_triggered()
    		{cleanup();}
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionShow_All_changed();
    void on_actionStay_On_Top_changed();
    inline void on_actionManual_triggered()
    		{QDesktopServices::openUrl(QUrl("https://sverrirvalgeirsson.github.io/Todour"));};    
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
  
   void on_actionSortAZ();
   void on_actionSortDate();
   
   void new_version(QString);

private:
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

    UGlobalHotkeys *hotkey;
    void setHotkey();

	QTimer *versionTimer;

    QString baseTitle;
    QSystemTrayIcon *trayicon = NULL;
    QMenu *traymenu=NULL;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    
    QModelIndex currentIndex;
	todour_version *Version;

    
//Gaetandc 4/1/24    
    QAction* actionPrint;

    QMenu* rClickMenu=NULL;
	    QAction* editAction;
   		QAction* deleteAction;
    	QAction* postponeAction;
    	QAction* duplicateAction;

    QMenu* priorityMenu=NULL;
    	QAction* ApriorAction;
    	QAction* BpriorAction;
    	QAction* CpriorAction;
    	QAction* DpriorAction;

	QMenu* sortMenu=NULL;    
    	QAction* sortAzAction;
    	QAction* sortDateAction;
    
    QSortFilterProxyModel *proxyModel;

};

#endif // MAINWINDOW_H
