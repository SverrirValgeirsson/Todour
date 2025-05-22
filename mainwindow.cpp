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

#define NEW_VERSION_STRING "<a href=\"http://nerdur.com/todour-pl\">http://nerdur.com/todour-pl</a>"


TodoTableModel *model=NULL;
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


// COMMENTED TO PREVENT SEG FAULT WITH WAYLAND
 // hotkey = new UGlobalHotkeys();
 //  setHotkey();

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

//    auto contextshortcut = new QShortcut(QKeySequence(tr("Ctrl+l")),this);
//    QObject::connect(contextshortcut,SIGNAL(activated()),ui->context_lock,SLOT(setChecked(!(ui->context_lock->isChecked()))));

 
    rClickMenu=new QMenu(this);
    	editAction = new QAction("&Edit", this);
		completeAction = new QAction("&Complete",this);
    	deleteAction = new QAction("&Delete", this);
    	postponeAction = new QAction("&Postpone +10p", this);
    	duplicateAction = new QAction("&Duplicate", this);

		connect(editAction, SIGNAL(triggered()), this, SLOT(on_actionEdit()));
		connect(completeAction, SIGNAL(triggered()), this, SLOT(on_actionComplete()));   
		connect(duplicateAction, SIGNAL(triggered()), this, SLOT(on_actionDuplicate()));
		connect(deleteAction, SIGNAL(triggered()), this, SLOT(on_actionDelete()));
		connect(postponeAction, SIGNAL(triggered()), this, SLOT(on_actionPostpone()));

		rClickMenu->addAction(deleteAction);
		rClickMenu->addAction(completeAction);
		rClickMenu->addAction(duplicateAction);
		rClickMenu->addAction(editAction);
		rClickMenu->addAction(postponeAction);

    priorityMenu=new QMenu("Priority",this);
	   rClickMenu->addMenu(priorityMenu);
	   ApriorAction = new QAction("(A) priority",this);
	   BpriorAction = new QAction("(B) priority",this);
	   CpriorAction = new QAction("(C) priority",this);
	   DpriorAction = new QAction("(D) priority",this);
    
		connect(ApriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityA()));
		connect(BpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityB()));
		connect(CpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityC()));
		connect(DpriorAction,SIGNAL(triggered()),this,SLOT(on_actionPriorityD()));

		priorityMenu->addAction(ApriorAction);
	    priorityMenu->addAction(BpriorAction);
    	priorityMenu->addAction(CpriorAction);
    	priorityMenu->addAction(DpriorAction);


	//undo
	_undoStack = new QUndoStack(this);
		undoAction = new QAction(tr("&Undo"),this);
		undoAction->setEnabled(_undoStack->canUndo());				
		undoAction->setIcon(QIcon(":/icons/undo.png"));
    	undoAction->setShortcuts(QKeySequence::Undo);
		connect(_undoStack, SIGNAL(canUndoChanged(bool)), undoAction, SLOT(setEnabled(bool)));
//	    connect(_undoStack, SIGNAL(undoTextChanged(QString)),undoAction, SLOT(setPrefixedText(QString)));
		connect(undoAction, SIGNAL(triggered()), this, SLOT(on_actionUndo()));
		
//    	redoAction = _undoStack->createRedoAction(this, tr("&Redo"));
		redoAction = new QAction(tr("&Redo"),this);
		redoAction->setEnabled(_undoStack->canRedo());				
		redoAction->setIcon(QIcon(":/icons/redo.png"));
    	redoAction->setShortcuts(QKeySequence::Redo);
		ui->menuEdit->addAction(undoAction);
		ui->menuEdit->addAction(redoAction);
		connect(_undoStack, SIGNAL(canRedoChanged(bool)), redoAction, SLOT(setEnabled(bool)));
//	    connect(_undoStack, SIGNAL(redoTextChanged(QString)),redoAction, SLOT(setText(QString)));
		connect(redoAction, SIGNAL(triggered()), this, SLOT(on_actionRedo()));
		
		
	// Copy:
		copyAction = new QAction(tr("&Copy"),this);
		copyAction->setShortcuts(QKeySequence::Copy);
	    ui->menuEdit->addAction(copyAction);
	    connect(copyAction,SIGNAL(triggered()),this,SLOT(on_actionCopy()));
	    
    sortMenu=new QMenu("sort",this);
	    sortAzAction = new QAction(tr("Sort alphabetically"),this);
    	sortDateAction = new QAction(tr("Sort by Input date"),this);
    	sortInactiveAction = new QAction(tr("Sort inactive last"),this);
    	sortAzAction->setCheckable(true);
    	sortDateAction->setCheckable(true);
    	sortInactiveAction->setCheckable(true);
    	
    	connect(sortAzAction, SIGNAL(triggered()), this, SLOT(on_actionSortAZ()));
    	connect(sortDateAction, SIGNAL(triggered()), this, SLOT(on_actionSortDate()));
    	connect(sortInactiveAction, SIGNAL(triggered()), this, SLOT(on_actionSortInactive()));    	

		sortMenu->addAction(sortAzAction);
		sortMenu->addAction(sortDateAction);
		sortMenu->addAction(sortInactiveAction);

    	ui->btn_Alphabetical->setMenu(sortMenu);
    	ui->btn_Alphabetical->setPopupMode( QToolButton::InstantPopup);
	
    // Version check
    Version = new todour_version();
    connect(Version,SIGNAL(NewVersion(QString)),this,SLOT(new_version(QString)));
	Version->onlineCheck(false);
	versionTimer = new QTimer(this);
	connect(versionTimer,SIGNAL(timeout()),this,SLOT(on_pb_closeVersionBar_clicked()));


    // Started. Lets open the todo.txt file, parse it and show it.
    model = new TodoTableModel(_undoStack,this);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterRole(Qt::UserRole);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->tableView->setModel(proxyModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView->resizeColumnToContents(0); // Checkboxes kept small
//	updateSearchResults();

    setFileWatch();
	ui->context_lock->setChecked(settings.value(SETTINGS_CONTEXT_LOCK,DEFAULT_CONTEXT_LOCK).toBool());

    ui->lv_activetags->hide(); //  Not being used yet
    ui->newVersionView->hide(); // This defaults to not being shown
    ui->actionShow_All->setChecked(settings.value(SETTINGS_SHOW_ALL,DEFAULT_SHOW_ALL).toBool());
    ui->actionStay_On_Top->setChecked(settings.value(SETTINGS_STAY_ON_TOP,DEFAULT_STAY_ON_TOP).toBool());
    ui->cb_threshold_inactive->setChecked(settings.value(SETTINGS_THRESHOLD_INACTIVE,DEFAULT_THRESHOLD_INACTIVE).toBool());
    stayOnTop();
    setTray();
    setFontSize();

		QStringList wordList;
		wordList << "alpha" << "omega" << "omicron" << "zeta";
		_taglist = new QCompleter(wordList, this);
		_taglist->setCaseSensitivity(Qt::CaseInsensitive);
		ui->lineEditFilter->setCompleter(_taglist);


    // Do this late as it triggers action using data
    //ui->btn_Alphabetical->setChecked(settings.value(SETTINGS_SORT_ALPHA).toBool());
    QObject::connect(model,SIGNAL(dataChanged (const QModelIndex , const QModelIndex )), this, 
    		SLOT(dataInModelChanged(QModelIndex,QModelIndex)));

    updateSearchResults(); // Since we may have set a value in the search window
	on_actionSortAZ();



	
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
{
    delete ui;
    delete model;
}

/* */
void MainWindow::updateTitle()
{
    if(proxyModel != NULL){
        int visible = proxyModel->rowCount();
        int total = proxyModel->sourceModel()->rowCount();
		if (_undoStack->isClean())
				this->setWindowTitle(baseTitle+" (" +QString::number(visible)+"/"+QString::number(total)+")");
		else 
				this->setWindowTitle(baseTitle+" * (" +QString::number(visible)+"/"+QString::number(total)+")");
	}
}

/* */
void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
	rClickMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

/* Disable the fileWatching. If disabled, the subsystem should not update the display if file is changed.
*/
void MainWindow::clearFileWatch()
{
	qDebug()<<" DEPRECATED :   MainWindow::clearFileWatch()"<<endline;
   model->clearFileWatch();
}

/* Activate the filewatching. when enabled, subsystem should monitor the file and keep the display up to date.
*/
void MainWindow::setFileWatch()
{
QSettings settings;
    model->clearFileWatch();
    if(! settings.value(SETTINGS_AUTOREFRESH).toBool())
           return;
   model->setFileWatch((QObject*) this);
}

void MainWindow::setHotkey(){
	//COMMENTED TO PREVENT SEGFAULT IN WAYLAND
	return;
//	QSettings settings;
//    if(settings.value(SETTINGS_HOTKEY_ENABLE).toBool()){
//        hotkey->registerHotkey(settings.value(SETTINGS_HOTKEY,DEFAULT_HOTKEY).toString());
//        connect(hotkey,&UGlobalHotkeys::activated,[=](size_t id){
//            Q_UNUSED(id);
//            on_hotkey();
//        }
//    else {
//        hotkey->unregisterAllHotkeys();
//    }

}


/* User edited the "filter" field. If liveupdate settings is activates, we have to inform the Proxymodel of the change
*/
void MainWindow::on_lineEditFilter_textEdited(const QString &arg1)
{
	QSettings settings;
    Q_UNUSED(arg1);
    if(!ui->actionShow_All->isChecked() && settings.value(SETTINGS_LIVE_SEARCH).toBool()){
        updateSearchResults();
    }
}

/* This is the "Show threshold" selection switch. 
	- when changed, refresh the list
	- when inactive, hide all "inactive" tasks
	- when active, show all "inactive" tasks
*/
void MainWindow::on_cb_threshold_inactive_stateChanged(int arg1)
{
	QSettings settings;
    settings.setValue(SETTINGS_THRESHOLD_INACTIVE,arg1);
	updateSearchResults();
}

/* 
*/
void MainWindow::on_lineEditFilter_returnPressed()
{
	QSettings settings;
    bool liveUpdate = settings.value(SETTINGS_LIVE_SEARCH).toBool();

    if(!liveUpdate || ui->actionShow_All->isChecked()){
        updateSearchResults();
    }
}

/* For any reason, the filters have changed (toggle_threshold, text filter, ...)
	We need to adapt.
*/
void MainWindow::updateSearchResults()
{
    // Take the text of the format of match1 match2 !match3 and turn it into
    //(?=.*match1)(?=.*match2)(?!.*match3) - all escaped of course   
//    qDebug()<<"MainWindow::updateSearchResults step 1"<<endline;
    
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
    
//    qDebug()<<" MainWindow::updateSearchResult: "<<regexpstring<<endline;
    QRegularExpression regexp(regexpstring);	
    proxyModel->setFilterRegularExpression(regexp);
    proxyModel->setFilterKeyColumn(1);
    updateTitle();
}

/* sorts the list A to Z*/
void MainWindow::on_actionSortAZ()
{
	sortDateAction->setChecked(false);
	sortAzAction->setChecked(true);

	updateSort();
}

/* sorts the list by input date DECREASING*/
void MainWindow::on_actionSortDate()
{
	sortAzAction->setChecked(false);
	sortDateAction->setChecked(true);
	updateSort();
}

/*
*/
void MainWindow::on_actionSortInactive()
{
	QSettings settings;
	settings.setValue(SETTINGS_SEPARATE_INACTIVES,sortInactiveAction->isChecked());
//	qDebug()<<"setting value = "<<settings.value(SETTINGS_SEPARATE_INACTIVES,DEFAULT_SEPARATE_INACTIVES).toBool()<<endline;
	updateSort();
	
}

void MainWindow::updateSort()
/* */
{
	proxyModel->invalidate();
	if(sortAzAction->isChecked()){
	qDebug()<<"updateSort case 1"<<endline;
		proxyModel->setSortRole(Qt::UserRole);
		proxyModel->sort(1,Qt::AscendingOrder);
	}
	else if(sortDateAction->isChecked()){
		qDebug()<<"updateSort case 2"<<endline;
		proxyModel->setSortRole(Qt::UserRole+1);
		proxyModel->sort(1,Qt::DescendingOrder);
	}
}

/* Copy the selected tasks to clipboard*/
void MainWindow::on_actionCopy()
{
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

/*
*/
void MainWindow::on_hotkey(){
    auto dlg = new QuickAddDialog();
    dlg->setModal(true);
    dlg->show();
    dlg->exec();
    if(dlg->accepted){
        this->addTodo(dlg->text,"");
    }
}

void MainWindow::on_actionAbout_triggered(){
    AboutBox d;
    d.setModal(true);
    d.show();
    d.exec();
    //myanalytics->check_update();
}

/* Opens settings dialog.
 */
void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog d;
    d.setModal(true);
    d.show();
    d.exec();

	model->refresh();
	updateTitle();
}

/* */
void MainWindow::setFontSize(){
	QSettings settings;
    int size = settings.value(SETTINGS_FONT_SIZE).toInt();
    if(size >0){
        auto f = qApp->font();
        f.setPointSize(size);
        qApp->setFont(f);
    }
}

/* */
void MainWindow::stayOnTop()
{
    if(ui->actionStay_On_Top->isChecked()){
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show(); // This is needed as setWindowFlags can hide the window
}

/* */
void MainWindow::setTray(){
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

/* */
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
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
/* */
{
    // Adding a new value into the model
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
/* #TODO the effective adding must heppen in the model.
*/
{
   	QSettings settings;
    QString prio="";
    task* t = new task(s,cont);
 	_undoStack->push(new AddCommand(model,t));
	model->refresh();
    updateTitle();
}

void MainWindow::on_archiveButton_clicked()
/* // Archive action.
*/
{
    model->archive();
    _undoStack->clear();

    updateTitle();
}

void MainWindow::on_refreshButton_clicked()
/*// This is the Refresh button
*/
{
    model->refresh();
    updateTitle();
}

void MainWindow::on_actionSave_triggered()
/* */
{
	model->flush();
    _undoStack->clear();
}

void MainWindow::cleanup()
/* */
{
	QSettings settings;
	qDebug()<<"Clean up ..."<<endline;	

	model->flush();
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

/*
*/
void MainWindow::closeEvent(QCloseEvent *ev){
    if (trayicon != NULL && trayicon->isVisible()) {
            hide();
            ev->ignore();
    } else {
        cleanup();
        ev->accept();
    }
}

/*
*/
void MainWindow::on_context_lock_toggled(bool checked)
{
	QSettings settings;
    settings.setValue(SETTINGS_CONTEXT_LOCK,checked);
}

/*
*/
void MainWindow::on_pb_closeVersionBar_clicked()
{
    ui->newVersionView->hide();
}


/*
*/
void MainWindow::on_actionShow_All_changed()
{
	QSettings settings;
    settings.setValue(SETTINGS_SHOW_ALL,ui->actionShow_All->isChecked());
    on_refreshButton_clicked();
}

/*
*/
void MainWindow::on_actionStay_On_Top_changed()
{
	QSettings settings;
    settings.setValue(SETTINGS_STAY_ON_TOP,ui->actionStay_On_Top->isChecked());
    stayOnTop();
}

void MainWindow::on_actionUndo()
/* */
{
	_undoStack->undo();
	model->refresh();
	updateTitle();
}

void MainWindow::on_actionRedo()
/* */
{
	_undoStack->redo();
	model->refresh();
	updateTitle();
}

void MainWindow::on_actionEdit()
/* User clicked on "Edit". We enable the editing of the line.
*/
{
   QModelIndex index = proxyModel->mapToSource(ui->tableView->selectionModel()->selection().indexes().first());
    if(!index.isValid()){
        ui->tableView->edit(index);
    }
}

void MainWindow::on_actionComplete()
/* user clicked "Complete" on a set of tasks. 
*/
{
	_undoStack->beginMacro("completion");	
	
    QModelIndexList indexes = ui->tableView->selectionModel()->selection().indexes();
	for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
		_undoStack->push(new CompleteCommand(model,model->getTask(proxyModel->mapToSource(*i)),true));
		}
		
	_undoStack->endMacro(); 
	model->refresh();   
    updateTitle();

}

void MainWindow::on_actionDelete()
/* User clicked on "Delete". We remove the selected items

	#TODO the undo_cmd should be in the model.
*/
{
    QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
//    qDebug()<<"MainWindow::on_actionDelete() indexes:"<<indexes<<endline;
    QList<QUuid> tuidL;
	if (!indexes.isEmpty()){
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
		    tuidL.push_back(model->getTask(*i)->getTuid());
		}

		_undoStack->beginMacro("deletion");			
		for (QList<QUuid>::iterator j=tuidL.begin();j!=tuidL.end();++j){
			_undoStack->push(new DeleteCommand(model,*j));
		}
		
		_undoStack->endMacro();    
		model->refresh();
    	updateTitle();
    }
}


void MainWindow::on_actionPostpone()
/* User clicked on Postpone. We postpone the task for a default value.
  Issue: make a setting for this default postpone.
  
  
  #TODO  the undo_cmd should be in the model
*/
{
// #TODO  Get the postpone value from QSettings...
    QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
 		_undoStack->beginMacro("postpone");			   
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			//model->postponeTasks(indexes,"t:+10d");
			_undoStack->push(new PostponeCommand(model, model->getTask(*i), " t:+10d"));
		}
		_undoStack->endMacro();    
		model->refresh();
	    updateTitle();
    }
}

void MainWindow::on_actionDuplicate()
/* User has clicked on "Duplicate". We need to make a copy of task and 

  #TODO  the undo_cmd should be in the model
*/
{
	QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
	if (! indexes.isEmpty()){
		_undoStack->beginMacro("duplicate");
		for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			_undoStack->push(new AddCommand(model,new task(model->getTask(*i))));
		}
		_undoStack->endMacro();   
		model->refresh(); 
		updateTitle();

	}
 }


void MainWindow::on_actionPriority(QChar p)
/* 
  #TODO  the undo_cmd should be in the model
*/
{
   QModelIndexList indexes = proxyModel->mapSelectionToSource(ui->tableView->selectionModel()->selection()).indexes();
    if(!indexes.empty()){
   		_undoStack->beginMacro("completion");
	    for (QList<QModelIndex>::iterator i=indexes.begin(); i!=indexes.end();++i){
			_undoStack->push(new PriorityCommand(model,model->getTask(*i), p));
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

/* The user has clicked on "Print". We print the selected tasks
*/
void MainWindow::on_actionPrint_triggered()
{
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
