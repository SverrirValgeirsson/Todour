#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "todotablemodel.h"

#include "settingsdialog.h"
#include "quickadddialog.h"
#include "aboutbox.h"
#include "def.h"

#include "todo_undo.h"

#include <QTime>
#include <QDebug>

#include <QShortcut>
#include <QCloseEvent>
#include <QtAwesome.h>	//used for fonts and icons
#include <QPalette>
#include <QDir>
#include <QSystemTrayIcon>
#include <QUuid>
#include <QPrinter> //used for printing
#include <QPrintDialog> //used for printing
#include <QTextDocument> //used for printing
#include <QGuiApplication>
#include <QActionGroup>
	
#define NEW_VERSION_STRING "<a href=\"http://nerdur.com/todour-pl\">http://nerdur.com/todour-pl</a>"

TodoTableModel *model=NULL;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	qDebug()<<"Start mainwindow creation..."<<endline;
    ui->setupUi(this);
    QString title=this->windowTitle();

    QCoreApplication::setOrganizationName("Nerdur");
    QCoreApplication::setOrganizationDomain("nerdur.com");
    QCoreApplication::setApplicationName("Todour");


#ifdef QT_NO_DEBUG
//    QCoreApplication::setOrganizationName("Nerdur");
//    QCoreApplication::setOrganizationDomain("nerdur.com");
//    QCoreApplication::setApplicationName("Todour");
//    title.append("-");
#else
//    QCoreApplication::setOrganizationName("Nerdur-debug");
//    QCoreApplication::setOrganizationDomain("nerdur-debug.com");
//    QCoreApplication::setApplicationName("Todour-Debug");
    title.append("-DEBUG-");
#endif
	QSettings settings;
	
    title.append(todour_version::get_version());
    baseTitle=title;
    this->setWindowTitle(title);


    // Check if we're supposed to have the settings from .ini file or not
    if(QCoreApplication::arguments().contains("-portable")){
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,QDir::currentPath());
        qDebug()<<"Setting ini file path to: "<<QDir::currentPath()<<endline;
    }

	if (QGuiApplication::platformName() == "xcb"){
		// only try to make hotkey on x11
 	//	 hotkey = new UGlobalHotkeys();
 	//	  setHotkey();
	}
    
    if(QCoreApplication::arguments().contains("--quickAdd") | QCoreApplication::arguments().contains("-a")){
    	// add a always working function. If todour is launched with -a flag, only show the "taskadd" and close.
    	this->on_hotkey();
    }

    // Restore the position of the window
    restoreGeometry(settings.value( SETTINGS_GEOMETRY, saveGeometry() ).toByteArray());
    restoreState(settings.value( SETTINGS_SAVESTATE, saveState() ).toByteArray());
    if ( settings.value( SETTINGS_MAXIMIZED, isMaximized() ).toBool() )
        showMaximized();

    // Check that we have an UUID for this application (used for undo for example)
    if(!settings.contains(SETTINGS_UUID)){
        settings.setValue(SETTINGS_UUID,QUuid::createUuid().toString());
    }

    // Fix some font-awesome stuff
    fa::QtAwesome* awesome = new fa::QtAwesome(qApp);
    awesome->initFontAwesome();     // This line is important as it loads the font and initializes the named icon map
    awesome->setDefaultOption("scale-factor",0.9);
    QVariantMap options;
	 //options.insert("color",QApplication::palette().color(QPalette::Normal, QPalette::ButtonText));
//	 options.insert("color-selected" , "#FF0000");
//	 options.insert("color-active" , "#FF0000");
//	 options.insert("color-selected-off" , "#FF0000");

	options.insert("color","red");
	options.insert("color-disabled","red");
	options.insert("color-active","red");
	options.insert("color-off",QApplication::palette().color(QPalette::Normal, QPalette::Text));
	options.insert("color-active-off",QApplication::palette().color(QPalette::Normal, QPalette::Text));

 
//	 options.insert("color-disabled" , QApplication::palette().color(QPalette::Normal, QPalette::Text));
	
    ui->btn_Alphabetical->setIcon(awesome->icon(fa::fa_solid, fa::fa_arrow_down_a_z ));
    ui->btn_Filter->setIcon(awesome->icon(fa::fa_solid, fa::fa_filter));
    ui->btn_Filter->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->archiveButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_right_from_bracket));
    ui->refreshButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_arrows_rotate ));
    ui->addButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_plus ));
    ui->context_lock->setIcon(awesome->icon(fa::fa_solid, fa::fa_lock, options));
    ui->pb_closeVersionBar->setIcon(awesome->icon(fa::fa_solid, fa::fa_xmark));
	 ui->deleteAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_trash));
	 ui->editAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_pencil));
	 ui->completeAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_check));
	 ui->duplicateAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_clone));
	 ui->postponeAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_forward));
	 ui->dueTodayAction->setIcon(awesome->icon(fa::fa_solid, fa::fa_plane));

	ui->actionSave->setShortcuts(QKeySequence::Save);
	ui->actionQuit->setShortcuts(QKeySequence::Quit);
	ui->actionQuit->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F4));
	ui->actionPrint->setShortcuts(QKeySequence::Print);
	ui->copyAction->setShortcuts(QKeySequence::Copy);
	
 

    // Set up shortcuts . Mac translates the Ctrl -> Cmd
    // http://doc.qt.io/qt-5/qshortcut.html
    auto editshortcut = new QShortcut(QKeySequence(tr("Ctrl+n")),this);
    QObject::connect(editshortcut,SIGNAL(activated()),ui->lineEditNew,SLOT(setFocus()));
    auto findshortcut = new QShortcut(QKeySequence(tr("Ctrl+f")),this);
    QObject::connect(findshortcut,SIGNAL(activated()),ui->lineEditFilter,SLOT(setFocus()));
    auto findshortcut2 = new QShortcut(QKeySequence(tr("F3")),this);
    ui->lineEditFilter->setPlaceholderText(tr("Search (F3)"));
    
    QObject::connect(findshortcut2,SIGNAL(activated()),ui->lineEditFilter,SLOT(setFocus()));
    auto switchshortcut = new QShortcut(QKeySequence(tr("F6")),this);
    QObject::connect(switchshortcut,SIGNAL(activated()),this, SLOT(toggleFocus()));

		connect(ui->editAction, SIGNAL(triggered()), this, SLOT(on_actionEdit()));
		connect(ui->completeAction, SIGNAL(triggered()), this, SLOT(on_actionComplete()));   
		connect(ui->duplicateAction, SIGNAL(triggered()), this, SLOT(on_actionDuplicate()));
		connect(ui->deleteAction, SIGNAL(triggered()), this, SLOT(on_actionDelete()));
		connect(ui->postponeAction, SIGNAL(triggered()), this, SLOT(on_actionPostpone()));
		connect(ui->dueTodayAction, SIGNAL(triggered()), this, SLOT(on_dueTodayAction_triggered()));		
		connect(ui->ApriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityA()));
		connect(ui->BpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityB()));
		connect(ui->CpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityC()));
		connect(ui->DpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityD()));
	   connect(ui->copyAction,SIGNAL(triggered()),this,SLOT(on_actionCopy()));
	//undo
	_undoStack = new QUndoStack(this);
		ui->undoAction->setEnabled(_undoStack->canUndo());				
    	ui->undoAction->setShortcuts(QKeySequence::Undo);
		connect(_undoStack, SIGNAL(canUndoChanged(bool)), ui->undoAction, SLOT(setEnabled(bool)));
		connect(ui->undoAction, SIGNAL(triggered()), this, SLOT(on_actionUndo()));
		
		ui->redoAction->setEnabled(_undoStack->canRedo());				
    	ui->redoAction->setShortcuts(QKeySequence::Redo);
		connect(_undoStack, SIGNAL(canRedoChanged(bool)), ui->redoAction, SLOT(setEnabled(bool)));
		connect(ui->redoAction, SIGNAL(triggered()), this, SLOT(on_actionRedo()));
	    	
    	connect(ui->sortAzAction, SIGNAL(triggered()), this, SLOT(updateSort()));
    	connect(ui->sortDateAction, SIGNAL(triggered()), this, SLOT(updateSort()));
    	connect(ui->sortInactiveAction, SIGNAL(triggered()), this, SLOT(updateSort()));    	
		
		QActionGroup* sortActionGroup = new QActionGroup(ui->btn_Alphabetical);
		ui->sortAzAction->setActionGroup(sortActionGroup);
		ui->sortDateAction->setActionGroup(sortActionGroup);
    	
    	connect(ui->defaultViewAction, SIGNAL(triggered()), this, SLOT(updateFilter()));    	
   	connect(ui->todaysViewAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
   	connect(ui->projectViewAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
   	connect(ui->showAllAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
    	connect(ui->respectThresholdDateAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
		connect(ui->respectThresholdContextAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
   	connect(ui->thresholdDueAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
   	connect(ui->hideInactiveAction, SIGNAL(triggered()), this, SLOT(updateFilter()));
    	
    	QActionGroup* viewActionGroup = new QActionGroup(ui->btn_Filter);
		ui->defaultViewAction->setActionGroup(viewActionGroup);
		ui->todaysViewAction->setActionGroup(viewActionGroup);
		ui->projectViewAction->setActionGroup(viewActionGroup);
		ui->showAllAction->setActionGroup(viewActionGroup);
 
    	connect(ui->lineEditFilter, SIGNAL(currentTextChanged(QString)),this, SLOT(updateFilterText(QString)));
 		connect(ui->lineEditNew, SIGNAL(returnPressed()),this, SLOT(on_addButton_clicked()));
    	    	
    	ui->btn_Alphabetical->setMenu(ui->sortMenu);
    	ui->btn_Alphabetical->setPopupMode( QToolButton::InstantPopup);
    	ui->btn_Filter->setMenu(ui->filterMenu);
    	ui->btn_Filter->setPopupMode( QToolButton::InstantPopup);
    	
	
    // Version check
    Version = new todour_version();
    connect(Version,SIGNAL(NewVersion(QString)),this,SLOT(new_version(QString)));
	Version->onlineCheck(false);
	versionTimer = new QTimer(this);
	connect(versionTimer,SIGNAL(timeout()),this,SLOT(on_pb_closeVersionBar_clicked()));
//	qDebug()<<"MainWindow init step 0"<<endline;

    // Started. Lets open the todo.txt file, parse it and show it.
   task_set = new taskset(this);
//	qDebug()<<"MainWindow init step 0.01"<<endline;
   model = new TodoTableModel(task_set,_undoStack, this);
// 	qDebug()<<"MainWindow init step 0.02"<<endline;
   proxyModel = new todoProxyModel(this);
   proxyModel->setSourceModel(model);
   ui->tableView->setModel(proxyModel);
   ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
   ui->tableView->resizeColumnToContents(0); // Checkboxes kept small
//	qDebug()<<"MainWindow init step 0.1"<<endline;
	connect(model,SIGNAL(dataChanged(QModelIndex, QModelIndex)),proxyModel,SLOT(invalidate()));
	note_set = new noteset(this);
	connect(note_set,SIGNAL(updateText(QString)),this,SLOT(handleNoteUpdate(QString)));
	note_set->reLoad();	
	
//	qDebug()<<"MainWindow init step 1"<<endline;
	ui->context_lock->setChecked(settings.value(SETTINGS_CONTEXT_LOCK,DEFAULT_CONTEXT_LOCK).toBool());
	ui->sortAzAction->setChecked(settings.value(SETTINGS_SORT_AZ,DEFAULT_SORT_AZ).toBool());
	ui->sortDateAction->setChecked(settings.value(SETTINGS_SORT_IDATE,DEFAULT_SORT_IDATE).toBool());
	
	ui->respectThresholdDateAction->setChecked(settings.value(SETTINGS_THRESHOLD_DATES,DEFAULT_THRESHOLD_DATES).toBool());
	ui->respectThresholdContextAction->setChecked(settings.value(SETTINGS_THRESHOLD_LABELS,DEFAULT_THRESHOLD_LABELS).toBool());
	ui->thresholdDueAction->setChecked(settings.value(	SETTINGS_DUE_AS_THRESHOLD,DEFAULT_DUE_AS_THRESHOLD).toBool());
	ui->hideInactiveAction->setChecked(settings.value(	SETTINGS_HIDE_INACTIVE,DEFAULT_HIDE_INACTIVE).toBool());
	
	ui->defaultViewAction->setChecked(true);
	ui->todaysViewAction->setChecked(settings.value(SETTINGS_TODAYS_VIEW,DEFAULT_TODAYS_VIEW).toBool());
	ui->projectViewAction->setChecked(settings.value(SETTINGS_PROJECT_VIEW,DEFAULT_PROJECT_VIEW).toBool());
	ui->showAllAction->setChecked(settings.value(SETTINGS_SHOW_ALL,DEFAULT_SHOW_ALL).toBool());
//	qDebug()<<"MainWindow init step 2"<<endline;

    ui->newVersionView->hide(); // This defaults to not being shown

    // Do this late as it triggers action using data
    QObject::connect(model,SIGNAL(dataChanged (const QModelIndex , const QModelIndex )), this, 
    		SLOT(dataInModelChanged(QModelIndex,QModelIndex)));

    QObject::connect(model,SIGNAL(dataChanged (const QModelIndex , const QModelIndex )),
    		proxyModel, SIGNAL(dataChanged (const QModelIndex , const QModelIndex )));

	QObject::connect(ui->actionSync,SIGNAL(triggered()),this,SLOT(on_actionSync_triggered()));

	autoSaver = new QTimer(this);
	QObject::connect(autoSaver,SIGNAL(timeout()),this,SLOT(on_actionSave_triggered()));
	// #TODO put this as a setting: autosave on/off?
	autoSaver->start(1000*60*15); // milliseconds

	QShortcut* spaceDone = new QShortcut(ui->tableView);
	spaceDone->setKey(Qt::Key_Space);
	spaceDone->setContext(Qt::WidgetShortcut);
	QObject::connect(spaceDone,SIGNAL(activated()),this,SLOT(on_actionSpace()));


	this->updateSettings();
	this->updateFilter(true);
	qDebug()<<"mainwindow initialised"<<endline;	
}


void MainWindow::updateSettings()
/* This function regroups all the layout issued from settingsdialog.
It is intended to be run at startup, and at closing of settings dialog.
It should be safe to run it at any time.
*/{
	QSettings settings;
      // Set some defaults if they dont exist
    if(!settings.contains(SETTINGS_LIVE_SEARCH))
        	settings.setValue(SETTINGS_LIVE_SEARCH,DEFAULT_LIVE_SEARCH);
    
	if (settings.value(SETTINGS_NOTE_ENABLE,DEFAULT_NOTE_ENABLE).toBool())
			ui->noteView->setVisible(true);
	else 
			ui->noteView->setVisible(false);
			
    ui->actionStay_On_Top->setChecked(settings.value(SETTINGS_STAY_ON_TOP,DEFAULT_STAY_ON_TOP).toBool());
    setTray();
    stayOnTop();
   
	//set font size
//    auto f = qApp->font();
//    f.setPointSize(settings.value(SETTINGS_FONT_SIZE,DEFAULT_FONT_SIZE).toInt());
//    qApp->setFont(f);
   
  	if ( settings.value(SETTINGS_ENHANCED_PM,DEFAULT_ENHANCED_PM).toBool()){
		ui->projectViewAction->setVisible(true);
		}
	else{
		ui->projectViewAction->setVisible(false);
		}

   
   
   task_set->setFileWatch(settings.value(SETTINGS_AUTOREFRESH).toBool(),(QObject*) this);

	ui->sortInactiveAction->setChecked(settings.value(SETTINGS_SEPARATE_INACTIVES,DEFAULT_SEPARATE_INACTIVES).toBool());
	ui->split->restoreState(settings.value(SETTING_SPLITTER_SIZE).toByteArray());
	
	updateSort();
	
	this->dataInModelChanged(QModelIndex(),QModelIndex());
	qDebug()<<"Mainwindow: finished update settings"<<endline;
	}


void MainWindow::dataInModelChanged(QModelIndex i1,QModelIndex i2)
/* dataInModelChanged informs us that data has changed.
We need to update the title + recalculate the tasks active.
*/
{
	QSettings settings;
    Q_UNUSED(i2)// one day, we can limit the computation to some subset...
    Q_UNUSED(i1)
    
	task_set->recalculate();
	QString filter = ui->lineEditFilter->currentText();
	ui->lineEditFilter->clear();
//	for (int i=0;i<ui->lineEditFilter->count();i++) ui->lineEditFilter->removeItem(i);
	ui->lineEditFilter->addItem("");

	if ( settings.value(SETTINGS_ENHANCED_PM,DEFAULT_ENHANCED_PM).toBool()){
		proxyModel->setContexts(task_set->getFullContexts());
		ui->lineEditFilter->addItems(task_set->getFullContexts());
		}
	else{
		proxyModel->setContexts(task_set->getContexts());
		ui->lineEditFilter->addItems(task_set->getContexts());
		}
	
	ui->lineEditFilter->setCurrentText(filter);
	
	//TODO: create a system to identify the change of context at lower level. Only update if changed.
	proxyModel->refresh();
	
	updateTitle();  
	}

MainWindow::~MainWindow()
/*
*/{
    delete ui;
    delete model;
    delete autoSaver;
    delete task_set;
    delete proxyModel;
    delete note_set;
    delete trayicon;
    
	}

void MainWindow::toggleFocus()
/* */
{
	if (!ui->noteView->isVisible()) return;
	if (!ui->noteView->hasFocus()) ui->noteView->setFocus(Qt::OtherFocusReason);
	else  ui->tableView->setFocus(Qt::OtherFocusReason);
}

void MainWindow::updateTitle()
/*
*/{
    if(proxyModel != NULL){
        int visible = proxyModel->rowCount();
        int total = proxyModel->sourceModel()->rowCount();
		if (_undoStack->isClean())
				this->setWindowTitle(baseTitle+" (" +QString::number(visible)+"/"+QString::number(total)+")");
		else 
				this->setWindowTitle(baseTitle+" * (" +QString::number(visible)+"/"+QString::number(total)+")");
		}
	}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
/*
*/{
	ui->rClickMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
	}

void MainWindow::on_noteView_customContextMenuRequested(const QPoint &pos)
/* NOT IMPLEMENTED
*/{
	Q_UNUSED(pos)
	}


void MainWindow::updateFilterText(QString arg1)
/* This function reads the filter submenu status + lineEditFilter text to give the right info to the proxy.
*/{
	QSettings settings;
    if(settings.value(SETTINGS_LIVE_SEARCH,DEFAULT_LIVE_SEARCH).toBool()){
        proxyModel->updateFilterText(arg1);
        updateTitle();
    }

}

void MainWindow::updateFilter(bool force)
/* This function reads the filter submenu status + lineEditFilter text to give the right info to the proxy.
*/{
	todoProxyModel::TodourFilterMode currentMode = proxyModel->getFilterMode();
	todoProxyModel::TodourFilterMode newval=proxyModel->getFilterMode();
	
	if (ui->todaysViewAction->isChecked() || ui->showAllAction->isChecked()) {
		ui->respectThresholdDateAction->setEnabled(false);
		ui->respectThresholdContextAction->setEnabled(false);
   	ui->thresholdDueAction->setEnabled(false);
   	ui->hideInactiveAction->setEnabled(false);
		}
	else {
		ui->respectThresholdDateAction->setEnabled(true);
		ui->respectThresholdContextAction->setEnabled(true);
   	ui->thresholdDueAction->setEnabled(true);
   	ui->hideInactiveAction->setEnabled(true);
		}

	if (ui->todaysViewAction->isChecked())
			newval |= todoProxyModel::TodaysView;
	else
			newval &=  ~todoProxyModel::TodaysView;

	if (ui->projectViewAction->isChecked())
			newval |= todoProxyModel::EnhancedPM;
	else
			newval &= ~todoProxyModel::EnhancedPM;	

	if (ui->respectThresholdDateAction->isChecked())
			newval |= todoProxyModel::HideThresholdDate;
	else
			newval &= ~todoProxyModel::HideThresholdDate;

	if (ui->respectThresholdContextAction->isChecked())
			newval |= todoProxyModel::HideThresholdContext;
	else
			newval &= ~todoProxyModel::HideThresholdContext;
		
	if (ui->thresholdDueAction->isChecked())
		newval |= todoProxyModel::HideUndue;
	else
		newval &= ~todoProxyModel::HideUndue;

	if (ui->hideInactiveAction->isChecked())
		newval |= todoProxyModel::HideInactive;
	else
		newval &= ~todoProxyModel::HideInactive;

	if (force || newval != currentMode)
			proxyModel->setFilterMode(newval);
	updateTitle();
}

void MainWindow::updateSort()
/* This function reads the sort submenu status to give the right info to the proxy.
*/{
//	qDebug()<<"MainWindow::updateSort()"<<endline;
	QSettings settings;
	todoProxyModel::TodourSortMode newval = proxyModel->getSortMode();
	
	if (ui->sortAzAction->isChecked()){
		newval |= todoProxyModel::sort_az;
		newval &= ~todoProxyModel::sort_idate;
		}
	else if (ui->sortDateAction->isChecked()){
		newval |= todoProxyModel::sort_idate;
		newval &=~todoProxyModel::sort_az;
		}

	if (ui->sortInactiveAction->isChecked()) //inactive last true
		newval |= todoProxyModel::inactive_last;
	else
		newval &= ~todoProxyModel::inactive_last;
	
	proxyModel->setSortMode(newval);

}

void MainWindow::setHotkey(){
	//COMMENTED TO PREVENT SEGFAULT IN WAYLAND
//	return;
	QSettings settings;
    if(settings.value(SETTINGS_HOTKEY_ENABLE).toBool()){
//        hotkey->registerHotkey(settings.value(SETTINGS_HOTKEY,DEFAULT_HOTKEY).toString());
//        connect(hotkey,&UGlobalHotkeys::activated,[=](size_t id){
//            Q_UNUSED(id);
//            on_hotkey();
        }
    else {
//        hotkey->unregisterAllHotkeys();
    	}

	}




void MainWindow::on_actionCopy()
/* Copy the selected tasks to clipboard
*/{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    if (!indexes.isEmpty()){
    	QString text = "";
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			text+=proxyModel->data(*i,Qt::EditRole).toString();
			text+="\n";
			}
		QClipboard *clipboard = QGuiApplication::clipboard();
		clipboard->setText(text, QClipboard::Clipboard);
		}
}

void MainWindow::on_hotkey()
/* */{
    auto dlg = new QuickAddDialog();
    dlg->setModal(true);
    dlg->show();
    dlg->exec();
    if(dlg->accepted){
        this->addTodo(dlg->text,"");
    }
}

void MainWindow::on_actionAbout_triggered()
/* */{
    AboutBox d;
    d.setModal(true);
    d.show();
    d.exec();
    //myanalytics->check_update();
}

void MainWindow::on_actionSettings_triggered()
/* Opens settings dialog.
 */{
    SettingsDialog d;
    d.setModal(true);
    d.show();
    d.exec();

	this->updateSettings();
	model->refresh();
}

void MainWindow::stayOnTop()
/* */{
    if(ui->actionStay_On_Top->isChecked()){
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show(); // This is needed as setWindowFlags can hide the window
}

void MainWindow::setTray()
/* */{
	QSettings settings;
    if(settings.value(SETTINGS_TRAY_ENABLED,DEFAULT_TRAY_ENABLED).toBool()){
        // We should be showing a tray icon. Are we?
        if(trayicon==NULL){
            trayicon = new QSystemTrayIcon(this);
            traymenu = new QMenu(this);
            minimizeAction = new QAction(tr("Mi&nimize"), this);
            connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
            maximizeAction = new QAction(tr("Ma&ximize"), this);
            connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
            restoreAction = new QAction(tr("&Restore"), this);
            connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
            quitAction = new QAction(tr("&Quit"), this);
            connect(quitAction, SIGNAL(triggered()), this, SLOT(cleanup()));
            connect(QApplication::instance(),SIGNAL(aboutToQuit()),this,SLOT(cleanup()));

            traymenu->addAction(minimizeAction);
            traymenu->addAction(maximizeAction);
            traymenu->addAction(restoreAction);
            traymenu->addSeparator();
            traymenu->addAction(quitAction);
            trayicon->setContextMenu(traymenu);
            connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

            trayicon->setIcon(QIcon(":/icons/todour.png"));
        }
        trayicon->show();
    }
    else{
        if(trayicon!=NULL){
            trayicon->hide();
        }
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
/* */{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
            // Make sure the window is open
            this->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        // Do nothing
        break;
    default:
        ;
    }
}

void MainWindow::on_addButton_clicked()
/* */{
	QString txt = ui->lineEditNew->text();
	QString context = "";
    if(ui->context_lock->isChecked()){
        // The line should have the context of the search field except any negative search
        QSettings settings;
        QChar not_char=QChar::Null;
        QString sett = settings.value(SETTINGS_SEARCH_NOT_CHAR,DEFAULT_SEARCH_NOT_CHAR).toString();
        if (sett.size()>0) sett.at(0);
        QStringList contexts = ui->lineEditFilter->currentText().split(QRegularExpression("\\s"));
        for(QString c:contexts){
         if(c.length()>0 && c.at(0)==not_char) continue; // ignore this one
         if(!context.contains(c,Qt::CaseInsensitive)){
             context.append(" "+c);
         }
        }
    }
    // Manage multiline input:
    if (txt.contains((QChar) QChar::LineFeed)){
    	QStringList mlines = txt.split((QChar) QChar::LineFeed, Qt::SkipEmptyParts);
    	for (QList<QString>::iterator it=mlines.begin();it!=mlines.end();++it){
    		addTodo(*it,context);
    		}
    	}
    else{
	   addTodo(txt,context);
	   }
   ui->lineEditNew->clear();
}

void MainWindow::addTodo(QString &s, QString cont)
/* 
*/{
	model->startModelChange();
	_undoStack->beginMacro("addition"); //do not change this text!
	model->safeAdd(s,cont);
	_undoStack->endMacro();
	model->endModelChange();
   updateTitle();
}

void MainWindow::on_archiveButton_clicked()
/* // Archive action.
*/{
	 model->startModelChange();
 	 task_set->archive();
 	 model->endModelChange();
    _undoStack->clear(); //no undo possible anymore.
    updateTitle();
}

void MainWindow::on_refreshButton_clicked()
/*// This is the Refresh button
*/{
    model->refresh();
    updateTitle();
}

void MainWindow::on_actionSave_triggered()
/* */{
	if (! _undoStack->isClean()){
		task_set->flush();
   	_undoStack->setClean();
   	}
   note_set->handleTextChanged(ui->noteView->toPlainText());
   	updateTitle();
}

void MainWindow::cleanup()
/* */{
	QSettings settings;
	qDebug()<<"Clean up ..."<<endline;	
	on_actionSave_triggered();
	
   settings.setValue(SETTINGS_THRESHOLD_DATES,ui->respectThresholdDateAction->isChecked());
   settings.setValue(SETTINGS_THRESHOLD_LABELS,ui->respectThresholdContextAction->isChecked());
	settings.setValue(SETTINGS_DUE_AS_THRESHOLD, ui->thresholdDueAction->isChecked());
	settings.setValue(SETTINGS_HIDE_INACTIVE, ui->hideInactiveAction->isChecked());
	settings.setValue(SETTINGS_SHOW_ALL, ui->showAllAction->isChecked());

	settings.setValue(SETTINGS_SORT_AZ,true);
	settings.setValue(SETTINGS_SORT_IDATE,false);
	settings.setValue(SETTINGS_SEPARATE_INACTIVES,ui->sortInactiveAction->isChecked());

   settings.setValue( SETTINGS_GEOMETRY, saveGeometry() );
   settings.setValue( SETTINGS_SAVESTATE, saveState() );
   settings.setValue( SETTINGS_MAXIMIZED, isMaximized() );
  	settings.setValue(SETTING_SPLITTER_SIZE, ui->split->saveState());

    settings.setValue(SETTINGS_CONTEXT_LOCK,ui->context_lock->isChecked());
    
   if(trayicon!=NULL){
   	delete trayicon;
   	trayicon = NULL;
   	}
    
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *ev)
/* */{
    if (trayicon != NULL && trayicon->isVisible()) {
		hide();
		ev->ignore();
    } else {
        cleanup();
        ev->accept();
    }
}

void MainWindow::on_pb_closeVersionBar_clicked()
/* */{
    ui->newVersionView->hide();
}

void MainWindow::on_actionStay_On_Top_changed()
/* */{
	QSettings settings;
    settings.setValue(SETTINGS_STAY_ON_TOP,ui->actionStay_On_Top->isChecked());
    stayOnTop();
}

void MainWindow::on_actionUndo()
/* */{
	if (_undoStack->undoText()=="deletion" || _undoStack->undoText()=="addition"){
	 	model->startModelChange();
		_undoStack->undo();
		model->endModelChange();
		}
	else _undoStack->undo();
	updateTitle();
}

void MainWindow::on_actionRedo()
/* */{
	if (_undoStack->redoText()=="deletion" || _undoStack->redoText()=="addition"){
	 	model->startModelChange();
		_undoStack->redo();
		model->endModelChange();
		}
	else _undoStack->redo();
	updateTitle();
}

void MainWindow::on_actionSpace()
/*  */{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
   	if(i->isValid())
	   	model->safeToggleComplete(*i);

	updateTitle();
   }
}

void MainWindow::on_actionEdit()
/* User clicked on "Edit". We enable the editing of the line.
*/{
   QModelIndex index = proxyModel->mapToSource(ui->tableView->selectionModel()->currentIndex());
    if(index.isValid())
     	   ui->tableView->edit(index);
}

void MainWindow::on_actionComplete()
/* user clicked "Complete" on a set of tasks. 
*/{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	_undoStack->beginMacro("Complete");
	for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
			model->safeComplete(*i,true);
	_undoStack->endMacro(); 
   updateTitle();
}

void MainWindow::on_actionDelete()
/* User clicked on "Delete". We remove the selected items
*/{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	model->safeDelete(indexes);
   updateTitle();

}

void MainWindow::on_actionPostpone()
/* User clicked on Postpone. We postpone the task for a default value.
  TODO: make a setting for this default postpone.  
*/{
    QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
  		_undoStack->beginMacro(tr("postpone"));
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
				model->safePostpone(*i,"t:+10d");
		_undoStack->endMacro(); 
	 updateTitle();
    }
}

void MainWindow::on_actionDuplicate()
/* User has clicked on "Duplicate". We need to make a copy of task and 
*/{
	QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	if (! indexes.isEmpty()){
		model->startModelChange();
 		_undoStack->beginMacro("addition"); // do not change this text
 		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
				model->safeAdd(task_set->at(i->row()));
		_undoStack->endMacro(); 
		model->endModelChange();		
		updateTitle();
	}
 }


void MainWindow::on_actionPriority(QChar p)
/* 
*/{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
  		_undoStack->beginMacro("priority");
	   for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
				model->safePriority(*i, p);
		_undoStack->endMacro(); 
	   updateTitle();
	 }
}

/* show "alarm" of new version available (status bar? Notification? balloon tooltip?)
*/
void MainWindow::new_version(QString text)
{
	ui->lbl_newVersion->setText(text+"  "+NEW_VERSION_STRING);
	ui->lbl_newVersion->setTextFormat(Qt::RichText);
	ui->lbl_newVersion->setOpenExternalLinks(true);
	ui->lbl_newVersion->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse); 	
	ui->newVersionView->show();
	versionTimer->start(10000);
}

void MainWindow::on_actionPrint_triggered()
/* The user has clicked on "Print". We print the selected tasks. If none is selected, print visible.
#TODO: consider printing the notes...
*/{
   auto selection = ui->tableView->selectionModel();
	QPrinter printer;
	QPrintDialog dialog(&printer, this);
	dialog.setWindowTitle(tr("Print Tasks"));
	if (dialog.exec() != QDialog::Accepted)
		return;
	QString txt_str;
	QModelIndexList list;
	if(selection->hasSelection())
			list=selection->selection().indexes();
	else {
		int rowCount = proxyModel->rowCount();
		list.reserve(rowCount);
		for (int row=0; row<rowCount;row++) list.append(proxyModel->index(row,1));
		}
	
	for (QList<QModelIndex>::iterator i=list.begin(); i!=list.end();++i)
	{
		txt_str=txt_str + "<br>";
		txt_str=txt_str+i->data(Qt::DisplayRole).toString();
	}
	QTextDocument text(this);
	text.setHtml(txt_str);
	text.print(&printer);
	
}

void MainWindow::on_actionSync_triggered()
/* Not implemented yet
  */
{qDebug()<<"Sync not implemented"<<endline;
}

void MainWindow::on_progressAction_triggered()
/*
*/{
	QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
   if(!indexes.empty()){
	   for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
				model->safeProgress(*i);
	   updateTitle();
	   }

}
void MainWindow::on_dueTodayAction_triggered()
/*
*/{
	QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
   if(!indexes.empty()){
	   for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i)
				model->safeDueDate(*i, QDateTime::currentDateTime());
	   updateTitle();
	   }

}

void MainWindow::handleNoteUpdate(QString txt)
/*
	note subsystem wants to change note content...
*/{
	ui->noteView->setPlainText(txt);
}
