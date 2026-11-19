#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
//#include <uglobalhotkeys.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer> //used for hiding version bar
#include <QDesktopServices> //used for showing the online user manual.
#include <QUndoStack>
#include <QClipboard>
#include <QCompleter>

#include <memory>

#include "version.h"
#include "taskset.h"
#include "todo_proxymodel.h"
#include "notetxt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void addTodo(QString &s, QString context);
    ~MainWindow();
    
public slots:
    void updateTitle();
    void handleNoteUpdate(QString txt);
    
private slots:
    void updateSettings();   
       
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionPrint_triggered();
    void on_actionSave_triggered();
	 void on_actionSync_triggered();
    inline void on_actionCheck_for_updates_triggered() {Version->onlineCheck(true);}
    inline void on_actionManual_triggered()
    		{QDesktopServices::openUrl(QUrl("https://sverrirvalgeirsson.github.io/Todour"));};    
    inline void on_actionQuit_triggered(){cleanup();}
	 void toggleFocus();

    void on_addButton_clicked();
    void on_archiveButton_clicked();
    void on_refreshButton_clicked();
    void on_pb_closeVersionBar_clicked();
    void on_progressAction_triggered();
	 void on_dueTodayAction_triggered();

    void on_hotkey();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cleanup(); // Need to have a quit slot of my own to save settings and so on.

    		
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void on_noteView_customContextMenuRequested(const QPoint &pos);
    		
    void on_actionStay_On_Top_changed();
    		
	 void dataInModelChanged(QModelIndex,QModelIndex);
    
   void on_actionEdit();
   void on_actionComplete();
   void on_actionDelete();
   void on_actionPostpone();
   void on_actionDuplicate();
   void on_actionPriority(QChar p);
	
   inline void on_actionPriorityA(){on_actionPriority('A');}
   inline void on_actionPriorityB(){on_actionPriority('B');}
   inline void on_actionPriorityC(){on_actionPriority('C');}
   inline void on_actionPriorityD(){on_actionPriority('D');}
  
   void updateFilter(bool force=false);
   void updateFilterText(QString);
   void updateSort();
   
   void on_actionCopy();
   void on_actionSpace();
   void on_actionUndo();
   void on_actionRedo();
    
   void new_version(QString);

private:
    todoProxyModel *proxyModel;

    void setTray();
    void closeEvent(QCloseEvent *ev);
    Ui::MainWindow *ui;
    void updateSearchResults();

    void stayOnTop();

//    UGlobalHotkeys *hotkey;   // Commented out for Wayland transition. Can be reintroduced if we can identify platform.
    void setHotkey();

	QTimer *versionTimer;
	QTimer *autoSaver;

    QString baseTitle;
    QSystemTrayIcon *trayicon = NULL;
    QMenu *traymenu=NULL;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QActionGroup* viewActionGroup;
	todour_version *Version;
	
	taskset* task_set;
	noteset* note_set;
	QUndoStack* _undoStack;
   bool enhancedPMMode; 
};
#endif // MAINWINDOW_H
