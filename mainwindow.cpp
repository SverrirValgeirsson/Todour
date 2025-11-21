#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "todotablemodel.h"

#include "settingsdialog.h"
#include "quickadddialog.h"
#include "aboutbox.h"
#include "def.h"

//#include "todo_undo.h"

#include <QTime>
#include <QDebug>
#include <QSettings>
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

#define NEW_VERSION_STRING "<a href=\"http://nerdur.com/todour-pl\">http://nerdur.com/todour-pl</a>"


//new objects: ideaView, syncButton

TodoTableModel *model=NULL;
IdeaTableModel* ideas=NULL;
QString saved_selection; // Used for selection memory

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

    ui->lineEditFilter->setText(settings.value(SETTINGS_SEARCH_STRING,DEFAULT_SEARCH_STRING).toString());

    // Check that we have an UUID for this application (used for undo for example)
    if(!settings.contains(SETTINGS_UUID)){
        settings.setValue(SETTINGS_UUID,QUuid::createUuid().toString());
    }

    // Fix some font-awesome stuff
    fa::QtAwesome* awesome = new fa::QtAwesome(qApp);
    awesome->initFontAwesome();     // This line is important as it loads the font and initializes the named icon map
    awesome->setDefaultOption("scale-factor",0.9);
    QVariantMap options;
	options.insert("color-active" , QColor(255, 0 ,0));
	options.insert("color-active-off",QApplication::palette().color(QPalette::Normal, QPalette::ButtonText));
	
    ui->btn_Alphabetical->setIcon(awesome->icon(fa::fa_solid, fa::fa_arrow_down_a_z ));
    ui->archiveButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_right_from_bracket));
    ui->refreshButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_arrows_rotate ));
    ui->addButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_plus ));
    ui->context_lock->setIcon(awesome->icon(fa::fa_solid, fa::fa_lock, options));
    ui->pb_closeVersionBar->setIcon(awesome->icon(fa::fa_solid, fa::fa_xmark));

	ui->actionSave->setShortcuts(QKeySequence::Save);
	ui->actionQuit->setShortcuts(QKeySequence::Quit);
	ui->actionPrint->setShortcuts(QKeySequence::Print);
	ui->copyAction->setShortcuts(QKeySequence::Copy);
	
    // Set some defaults if they dont exist
    if(!settings.contains(SETTINGS_LIVE_SEARCH)){
        settings.setValue(SETTINGS_LIVE_SEARCH,DEFAULT_LIVE_SEARCH);
    }


    // Set up shortcuts . Mac translates the Ctrl -> Cmd
    // http://doc.qt.io/qt-5/qshortcut.html
    auto editshortcut = new QShortcut(QKeySequence(tr("Ctrl+n")),this);
    QObject::connect(editshortcut,SIGNAL(activated()),ui->lineEditNew,SLOT(setFocus()));
    auto findshortcut = new QShortcut(QKeySequence(tr("Ctrl+f")),this);
    QObject::connect(findshortcut,SIGNAL(activated()),ui->lineEditFilter,SLOT(setFocus()));
    auto findshortcut2 = new QShortcut(QKeySequence(tr("F3")),this);
    QObject::connect(findshortcut2,SIGNAL(activated()),ui->lineEditFilter,SLOT(setFocus()));

		connect(ui->editAction, SIGNAL(triggered()), this, SLOT(on_actionEdit()));
		connect(ui->completeAction, SIGNAL(triggered()), this, SLOT(on_actionComplete()));   
		connect(ui->duplicateAction, SIGNAL(triggered()), this, SLOT(on_actionDuplicate()));
		connect(ui->deleteAction, SIGNAL(triggered()), this, SLOT(on_actionDelete()));
		connect(ui->postponeAction, SIGNAL(triggered()), this, SLOT(on_actionPostpone()));
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
	    	
    	connect(ui->sortAzAction, SIGNAL(triggered()), this, SLOT(on_actionSortAZ()));
    	connect(ui->sortDateAction, SIGNAL(triggered()), this, SLOT(on_actionSortDate()));
    	connect(ui->sortInactiveAction, SIGNAL(triggered()), this, SLOT(on_actionSortInactive()));    	
    	
    	ui->btn_Alphabetical->setMenu(ui->sortMenu);
    	ui->btn_Alphabetical->setPopupMode( QToolButton::InstantPopup);
	
    // Version check
    Version = new todour_version();
    connect(Version,SIGNAL(NewVersion(QString)),this,SLOT(new_version(QString)));
	Version->onlineCheck(false);
	versionTimer = new QTimer(this);
	connect(versionTimer,SIGNAL(timeout()),this,SLOT(on_pb_closeVersionBar_clicked()));


    // Started. Lets open the todo.txt file, parse it and show it.
    task_set = new taskset(_undoStack,this);
    model = new TodoTableModel(task_set,this);
    ideas = new IdeaTableModel(task_set, this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterRole(Qt::UserRole);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(proxyModel);
    
    ideaProxyModel = new QSortFilterProxyModel(this);
    ideaProxyModel->setSourceModel(ideas);
    ideaProxyModel->setFilterRole(Qt::UserRole);
    ideaProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->ideaView->setModel(ideaProxyModel);
    ideaProxyModel->setFilterRegularExpression(QString("^((?!")+QString(TODOUR_INACTIVE)+QString(").)*$"));
    ideaProxyModel->setFilterKeyColumn(0);

    
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView->resizeColumnToContents(0); // Checkboxes kept small
//    ui->ideaView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
//    ui->ideaView->resizeColumnToContents(0); // Checkboxes kept small

//	updateSearchResults();

    setFileWatch();
	ui->context_lock->setChecked(settings.value(SETTINGS_CONTEXT_LOCK,DEFAULT_CONTEXT_LOCK).toBool());

//    ui->lv_activetags->hide(); //  Not being used yet
    ui->newVersionView->hide(); // This defaults to not being shown
    ui->actionShow_All->setChecked(settings.value(SETTINGS_SHOW_ALL,DEFAULT_SHOW_ALL).toBool());
    ui->actionStay_On_Top->setChecked(settings.value(SETTINGS_STAY_ON_TOP,DEFAULT_STAY_ON_TOP).toBool());
    ui->cb_threshold_inactive->setChecked(settings.value(SETTINGS_THRESHOLD_INACTIVE,DEFAULT_THRESHOLD_INACTIVE).toBool());
    stayOnTop();
    setTray();
    setFontSize();

	//	QStringList wordList;
	//	wordList << "alpha" << "omega" << "omicron" << "zeta";
	//	_taglist = new QCompleter(wordList, this);
	//	_taglist->setCaseSensitivity(Qt::CaseInsensitive);
	//	ui->lineEditFilter->setCompleter(_taglist);


    // Do this late as it triggers action using data
    //ui->btn_Alphabetical->setChecked(settings.value(SETTINGS_SORT_ALPHA).toBool());
    QObject::connect(model,SIGNAL(dataChanged (const QModelIndex , const QModelIndex )), this, 
    		SLOT(dataInModelChanged(QModelIndex,QModelIndex)));
    QObject::connect(ideas,SIGNAL(dataChanged (const QModelIndex , const QModelIndex )), this, 
    		SLOT(dataInModelChanged(QModelIndex,QModelIndex)));

    updateSearchResults(); // Since we may have set a value in the search window
	on_actionSortAZ();

	QObject::connect(ui->actionSync,SIGNAL(triggered()),this,SLOT(on_actionSync_triggered()));

	autoSaver = new QTimer(this);
	QObject::connect(autoSaver,SIGNAL(timeout()),this,SLOT(on_actionSave_triggered()));
	// #TODO put this as a setting: autosave on/off?
	autoSaver->start(1000*60*15); // milliseconds

	QShortcut* spaceDone = new QShortcut(ui->tableView);
	spaceDone->setKey(Qt::Key_Space);
	spaceDone->setContext(Qt::WidgetShortcut);
	QObject::connect(spaceDone,SIGNAL(activated()),this,SLOT(on_actionSpace()));

	qDebug()<<"mainwindow initialised"<<endline;	
}

void MainWindow::dataInModelChanged(QModelIndex i1,QModelIndex i2)
/* dataInModelChanged informs us that data has changed.
We need to update the title + ?
*/
{
    Q_UNUSED(i2);
    Q_UNUSED(i1);
	this->updateTitle();
    
}

MainWindow::~MainWindow()
/*
*/{
    delete ui;
    delete model;
    delete ideas;
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

/* Activate the filewatching. when enabled, subsystem should monitor the file and keep the display up to date.
*/
void MainWindow::setFileWatch()
/*
*/{
QSettings settings;
//    model->clearFileWatch();
//    if(! settings.value(SETTINGS_AUTOREFRESH).toBool())
//           return;
   task_set->setFileWatch(settings.value(SETTINGS_AUTOREFRESH).toBool(),(QObject*) this);
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


void MainWindow::on_lineEditFilter_textEdited(const QString &arg1)
/* User edited the "filter" field. If liveupdate settings is activates, we have to inform the Proxymodel of the change
*/{
	QSettings settings;
    Q_UNUSED(arg1);
    if(!ui->actionShow_All->isChecked() && settings.value(SETTINGS_LIVE_SEARCH).toBool()){
        updateSearchResults();
    }
}

void MainWindow::on_cb_threshold_inactive_stateChanged(int arg1)
/* This is the "Show threshold" selection switch. 
	- when changed, refresh the list
	- when inactive, hide all "inactive" tasks
	- when active, show all "inactive" tasks
*/{
	QSettings settings;
    settings.setValue(SETTINGS_THRESHOLD_INACTIVE,arg1);
	updateSearchResults();
}

void MainWindow::on_lineEditFilter_returnPressed()
/* 
*/{
	QSettings settings;
    bool liveUpdate = settings.value(SETTINGS_LIVE_SEARCH).toBool();

    if(!liveUpdate || ui->actionShow_All->isChecked()){
        updateSearchResults();
    }
}

void MainWindow::updateSearchResults()
/* For any reason, the filters have changed (toggle_threshold, text filter, ...)
	We need to adapt.
*/{
    // Take the text of the format of match1 match2 !match3 and turn it into
    //(?=.*match1)(?=.*match2)(?!.*match3) - all escaped of course   
	QSettings settings;    
    QChar search_not_char = settings.value(SETTINGS_SEARCH_NOT_CHAR,DEFAULT_SEARCH_NOT_CHAR).toChar();
    QStringList words = ui->lineEditFilter->text().split(QRegularExpression("\\s+"));
    bool show_thr = (ui->cb_threshold_inactive->checkState() == Qt::Checked);
 	Q_UNUSED(show_thr);
    QString regexpstring="(?=^.*$)"; // Seems a negative lookahead can't be first (!?), so this is a workaround
    #define START "(?=^.*"
    #define STARTN "(?!^.*"
    for(QString word:words){
        if(word.length()==0) break;

        if(word.at(0)==search_not_char){
        	regexpstring += STARTN+QRegularExpression::escape(word.remove(0,1))+".*$)";
        }else{
	        regexpstring += START+QRegularExpression::escape(word)+".*$)";
    	}
    }

    if (!settings.value(SETTINGS_THRESHOLD_INACTIVE,DEFAULT_THRESHOLD_INACTIVE).toBool() 
    			|| ui->cb_threshold_inactive->checkState()==Qt::Unchecked)
    {
    	regexpstring +=START;
    	regexpstring +=TODOUR_INACTIVE;
    	regexpstring +=".*$)";	 
    } 

    QRegularExpression regexp(regexpstring);	
    proxyModel->setFilterRegularExpression(regexp);
    proxyModel->setFilterKeyColumn(1);
    updateTitle();
}

void MainWindow::on_actionSortAZ()
/* sorts the list A to Z
*/{
	ui->sortDateAction->setChecked(false);
	ui->sortAzAction->setChecked(true);

	updateSort();
}

void MainWindow::on_actionSortDate()
/* sorts the list by input date DECREASING
*/{
	ui->sortAzAction->setChecked(false);
	ui->sortDateAction->setChecked(true);
	updateSort();
}

void MainWindow::on_actionSortInactive()
/* 
*/{
	QSettings settings;
	settings.setValue(SETTINGS_SEPARATE_INACTIVES,ui->sortInactiveAction->isChecked());
//	qDebug()<<"setting value = "<<settings.value(SETTINGS_SEPARATE_INACTIVES,DEFAULT_SEPARATE_INACTIVES).toBool()<<endline;
	updateSort();
	
}

void MainWindow::updateSort()
/* 
*/{
	proxyModel->invalidate();
	if(ui->sortAzAction->isChecked()){
	qDebug()<<"updateSort case 1"<<endline;
		proxyModel->setSortRole(Qt::UserRole);
		proxyModel->sort(1,Qt::AscendingOrder);
	}
	else if(ui->sortDateAction->isChecked()){
		qDebug()<<"updateSort case 2"<<endline;
		proxyModel->setSortRole(Qt::UserRole+1);
		proxyModel->sort(1,Qt::DescendingOrder);
	}
}

void MainWindow::on_actionCopy()
/* Copy the selected tasks to clipboard*/{
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
    if (!indexes.isEmpty()){
    	QString text = "";
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			text+=model->getTask(proxyModel->mapToSource(*i))->getEditText();
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

	model->refresh();
	updateTitle();
}

void MainWindow::setFontSize()
/* */{
	QSettings settings;
    int size = settings.value(SETTINGS_FONT_SIZE).toInt();
    if(size >0){
        auto f = qApp->font();
        f.setPointSize(size);
        qApp->setFont(f);
    }
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
   QSettings settings;
	QString txt = ui->lineEditNew->text();
	QString context = "";
    if(ui->context_lock->isChecked()){
        // The line should have the context of the search field except any negative search
        QStringList contexts = ui->lineEditFilter->text().split(QRegularExpression("\\s"));
        for(QString c:contexts){
         if(c.length()>0 && c.at(0)=='!') continue; // ignore this one
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
	QSettings settings;
	task* t = new task(s,cont);
	task_set->safeAdd(t);

	model->refresh();
	ideas->refresh();
   updateTitle();
}

void MainWindow::on_archiveButton_clicked()
/* // Archive action.
*/{
//TODO: refresh the view
	task_set->archive();
    _undoStack->clear(); //no undo possible anymore.
	
    updateTitle();
}

void MainWindow::on_refreshButton_clicked()
/*// This is the Refresh button
*/{
    model->refresh();
    ideas->refresh();
    updateTitle();
}

void MainWindow:: on_syncButton_clicked()
/* This is the sync button. It should initiate a sync, according to QSettings + eventually ask a password
TODO: this is currently only used for testing the second task window.
*/{
	if (ui->ideaView->isVisible()){
		ui->ideaView->setVisible(false);
		ui->cb_threshold_inactive->setVisible(true);}
	else {
		ui->ideaView->setVisible(true	);
		ui->cb_threshold_inactive->setVisible(false);}
//	ui->actionSync->trigger();
	}


void MainWindow::on_actionSave_triggered()
/* */{
	if (_undoStack->isClean()) //nothing to do
			return;
	task_set->flush();
    _undoStack->setClean();
}

void MainWindow::cleanup()
/* */{
	QSettings settings;
	qDebug()<<"Clean up ..."<<endline;	

	task_set->flush();
    settings.setValue( SETTINGS_GEOMETRY, saveGeometry() );
    settings.setValue( SETTINGS_SAVESTATE, saveState() );
    settings.setValue( SETTINGS_MAXIMIZED, isMaximized() );
    settings.setValue(SETTINGS_SEARCH_STRING,ui->lineEditFilter->text());
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

void MainWindow::on_context_lock_toggled(bool checked)
/* */{
	QSettings settings;
    settings.setValue(SETTINGS_CONTEXT_LOCK,checked);
}

void MainWindow::on_pb_closeVersionBar_clicked()
/* */{
    ui->newVersionView->hide();
}

void MainWindow::on_actionShow_All_changed()
/* */{
	QSettings settings;
    settings.setValue(SETTINGS_SHOW_ALL,ui->actionShow_All->isChecked());
    on_refreshButton_clicked();
}

void MainWindow::on_actionStay_On_Top_changed()
/* */{
	QSettings settings;
    settings.setValue(SETTINGS_STAY_ON_TOP,ui->actionStay_On_Top->isChecked());
    stayOnTop();
}

void MainWindow::on_actionUndo()
/* */{
	_undoStack->undo();
	model->refresh();
	ideas->refresh();
	updateTitle();
}

void MainWindow::on_actionRedo()
/* */{
	_undoStack->redo();
	model->refresh();
	ideas->refresh();
	updateTitle();
}

void MainWindow::on_actionSpace()
/*  */{
   QModelIndex index = proxyModel->mapToSource(ui->tableView->selectionModel()->currentIndex());
    if(index.isValid()){
        //model->toggleDone(index);
        ui->tableView->selectionModel()->setCurrentIndex(proxyModel->mapFromSource(index), QItemSelectionModel::SelectCurrent);
    }
    
}

void MainWindow::on_actionEdit()
/* User clicked on "Edit". We enable the editing of the line.
*/{
// #TODO: check that the selection is valid before using it
   QModelIndex index = proxyModel->mapToSource(ui->tableView->selectionModel()->currentIndex());
    if(index.isValid()){
        ui->tableView->edit(index);
    }
}

void MainWindow::on_actionComplete()
/* user clicked "Complete" on a set of tasks. 
*/{
	_undoStack->beginMacro("completion");
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
	for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
		task_set->safeComplete((proxyModel->mapToSource(*i)).row(),true);
		}
		
	_undoStack->endMacro(); 
	model->refresh();
    updateTitle();

}

void MainWindow::on_actionDelete()
/* User clicked on "Delete". We remove the selected items
*/{
    QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    QList<QUuid> tuidL;
	if (!indexes.isEmpty()){
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
		    tuidL.push_back(model->getTask(*i)->getTuid());
		}

		_undoStack->beginMacro("deletion");			
		for (QList<QUuid>::iterator j=tuidL.begin();j!=tuidL.end();++j){
			task_set->safeDelete(*j);
		}
		
		_undoStack->endMacro();    
		model->refresh();
		ideas->refresh();
    	updateTitle();
    }
}


void MainWindow::on_actionPostpone()
/* User clicked on Postpone. We postpone the task for a default value.
  TODO: make a setting for this default postpone.  
*/{
    QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
 		_undoStack->beginMacro("postpone");			   
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			task_set->safePostpone(i->row(),"t:+10d");
		}
		_undoStack->endMacro();    
		model->refresh();
	    updateTitle();
    }
}

void MainWindow::on_actionDuplicate()
/* User has clicked on "Duplicate". We need to make a copy of task and 
*/{
	QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	if (! indexes.isEmpty()){
		_undoStack->beginMacro("duplicate");
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			task_set->safeAdd(new task(task_set->at(i->row())));
		}
		_undoStack->endMacro();   
		model->refresh(); 
		ideas->refresh();
		updateTitle();

	}
 }


void MainWindow::on_actionPriority(QChar p)
/* 
  #TODO  the undo_cmd should be in the model
*/{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
   		_undoStack->beginMacro("completion");
	    for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			task_set->safePriority(i->row(), p);
		}

		_undoStack->endMacro();   
	    model->refresh();
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
/* The user has clicked on "Print". We print the selected tasks
*/{
    auto selection = ui->tableView->selectionModel();
    if(selection->hasSelection()){
		QPrinter printer;
		
		QPrintDialog dialog(&printer, this);
		dialog.setWindowTitle(tr("Print Tasks"));
		if (dialog.exec() != QDialog::Accepted)
			return;
		QString txt_str;
		QModelIndexList list=selection->selection().indexes();
		for (QList<QModelIndex>::iterator i=list.begin(); i!=list.end();++i)
		{
			txt_str=txt_str + "<br>";
			txt_str=txt_str+i->data(Qt::DisplayRole).toString();
		}
		QTextDocument text(this);
		text.setHtml(txt_str);
		text.print(&printer);
	}
}

void MainWindow::on_actionSync_triggered()
/*The user has clicked on "synchronize. We now should
	1. try to connect to the caldav server in settings. 
  		if error, show a message and stop
	2. get list of tasks from server
		+ for each local & remote tasks, check timestamp
	3. update local or remote
	
	QUESTIONS: 
	- where is the best place to do each task? not here...
	- we added a TUID to the tasks, but there is no timestamp...  :-(
  */
{

}

