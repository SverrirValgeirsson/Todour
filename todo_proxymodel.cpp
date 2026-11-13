#include <QSettings>
#include "todo_proxymodel.h"
#include <QDebug>

todoProxyModel::todoProxyModel(QObject *parent)
	:QSortFilterProxyModel(parent)
/*
*/{
  	this->actual_sort = todoProxyModel::sort_az;
	this->actual_filter= todoProxyModel::NoFilter;

	this->setFilterCaseSensitivity(Qt::CaseInsensitive);
	this->setFilterRole(Qt::DisplayRole);
	this->setFilterKeyColumn(1);
   filterText.setPattern("(?=^.*$)");
   filterText.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
   filterTextHasProject = false;
   this->setFilterRegularExpression(filterText);
   setDynamicSortFilter(false);
}

todoProxyModel::~todoProxyModel()
	/*
*/{}



 bool todoProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
/* This is where we select the ordering. if left<right => return TRUE, otherwise return FALSE
we order based on
- order-by-az selected
- order-by-input-date selected
- inactive-last selected or not

Qt::UserRole+7 = completed
Qt::UserRole+2 = active
*/{
//	qDebug()<<"todoProxyModel::lessThan"<<endline;
	 	if (actual_sort.testFlag(todoProxyModel::inactive_last)){
	 		if (sourceModel()->data(left,Qt::UserRole+2).toBool() ^ (sourceModel()->data(right,Qt::UserRole+2).toBool()))
					return (sourceModel()->data(left,Qt::UserRole+2).toBool()); // sort inactive last
			
			if (sourceModel()->data(left,Qt::UserRole+7).toBool() ^ (sourceModel()->data(right,Qt::UserRole+7).toBool()))
					return (sourceModel()->data(right,Qt::UserRole+7).toBool()); // sort done last		
			}

		if (actual_sort.testFlag(todoProxyModel::sort_az))
					return QString::localeAwareCompare(sourceModel()->data(left,Qt::DisplayRole).toString(), sourceModel()->data(right,Qt::DisplayRole).toString())<0;   	
		
		if (actual_sort.testFlag(todoProxyModel::sort_idate))
		    	return sourceModel()->data(left,Qt::UserRole+1).toDateTime() > sourceModel()->data(right,Qt::UserRole+1).toDateTime();

	
	qDebug()<<" todoProxyModel::sort - erreur sorting"<<endline;       
	return QSortFilterProxyModel::lessThan(left,right);
}

 bool todoProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
/* This is the function to filter. Depending on our internal settings + the value of sourceParent(sourceRow), we says yes or no.
We filter based on
- the filtertext : by parent class
- the "show inactive" selection (passed to here)
- the threshold dates
- the threshold contexts
- the "treat due as threshold" ?
- the "today's view". We should be getting a score from (??) 
*/{
	QModelIndex source_index = sourceModel()->index(sourceRow, 1, sourceParent);
	QDateTime t_dueDate = sourceModel()->data(source_index,Qt::UserRole+4).toDateTime();

// if ShowAll, rely on text.
	if (actual_filter.testFlag(todoProxyModel::ShowAll))
			return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);

// hide inactive if not ShowInactive
	if (actual_filter.testFlag(todoProxyModel::HideInactive) ||actual_filter.testFlag(todoProxyModel::TodaysView))
			if (!sourceModel()->data(source_index,Qt::UserRole+2).toBool())// isActive()
					if (!(t_dueDate.isValid() && t_dueDate < dueWarningDate))	return false;

			// Threshold date.
	if ((actual_filter.testFlag(todoProxyModel::HideThresholdDate) ||actual_filter.testFlag(todoProxyModel::TodaysView)) && 
			sourceModel()->data(source_index,Qt::UserRole+3).toDateTime().isValid() &&
			sourceModel()->data(source_index,Qt::UserRole+3).toDateTime()>QDateTime::currentDateTime())
					if (!(t_dueDate.isValid() && t_dueDate < dueWarningDate))	return false;
			// Due as Threshold
	if (actual_filter.testFlag(todoProxyModel::HideUndue) && 
			t_dueDate.isValid() &&
			t_dueDate > dueWarningDate)
					if (!(t_dueDate.isValid() && t_dueDate < dueWarningDate))	return false;


// if ENHANCED_PM mode
	if (actual_filter.testFlag(todoProxyModel::EnhancedPM)){
	// 2.1 if a +project is selected, only show that one.
		if (filterTextHasProject)//the filtertext contains a project (+...)
				return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);
		else{ // else, hide all the +
			if (t_dueDate.isValid() && t_dueDate < dueWarningDate)
					return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);
			if (!actual_filter.testFlag(todoProxyModel::HideThresholdContext))
					return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);
			if ( (! sourceModel()->data(source_index,Qt::UserRole+5).toStringList().empty())
					|| (! sourceModel()->data(source_index,Qt::UserRole+8).toStringList().empty() ) )
							return false;					
			}
		}
	else { // no enhancedPM, 
		if (actual_filter.testFlag(todoProxyModel::HideThresholdContext)){
//			qDebug()<<"filtering: reject context "<<contexts<<sourceModel()->data(source_index,Qt::UserRole+5).toStringList()<<endline;
			for (QString i:contexts){
				if (sourceModel()->data(source_index,Qt::UserRole+5).toStringList().contains(i)  //UserRole+5=threshold_context.
//						|| sourceModel()->data(source_index,Qt::UserRole+8).toStringList().contains(i)
						)
							return false;
				}
			}				
		}
	if (actual_filter.testFlag(todoProxyModel::TodaysView)){ // hide inactive, threshold, D-priority, context-threshold, complete
		if (sourceModel()->data(source_index,Qt::UserRole+7) == Qt::Checked) return false; // no Checked task
		if (sourceModel()->data(source_index,Qt::UserRole+6).toChar()>'C')	return false; // Priority must be low
	}

	
	return QSortFilterProxyModel::filterAcceptsRow(sourceRow,sourceParent);

}

void todoProxyModel::refresh()
/* Ensure the filtering and sorting are updated
*/{
	this->invalidate();  //filter
	this->sort(1,Qt::AscendingOrder); //sorting
}


void todoProxyModel::setSortMode(TodourSortMode mode)
/* 
*/{
	actual_sort = mode;
	refresh();

}

void todoProxyModel::setFilterMode(TodourFilterMode mode)
/*
*/{

//	qDebug()<<"setFilterMode: new filter="<<mode<<endline;

	actual_filter=mode;

	QSettings settings;	
	beginFilterChange();
	dueWarningDate=QDateTime::currentDateTime().addDays(settings.value(SETTINGS_DUE_WARNING,DEFAULT_DUE_WARNING).toInt());
   endFilterChange(QSortFilterProxyModel::Direction::Rows);
	refresh();
}

void todoProxyModel::addFilterMode(TodourFilterMode mode)
/*TODO: this needs some check for incompatible modes, if any.
*/{
	actual_filter |= mode;
	refresh();
}


void todoProxyModel::setContexts(QStringList newc)
/*
*/{
	this->contexts = newc;
	refresh();
}


void todoProxyModel::updateFilterText(QString filter)
/* For any reason, the filters have changed (toggle_threshold, text filter, ...)
	We need to adapt.
*/{
	QSettings settings;    
  	filterTextHasProject = false;
   
//	qDebug()<<"todoProxyModel::updateFilterText  previous filter="<<this->filterRegularExpression()<<endline;
		
   // Take the text of the format of match1 match2 !match3 and turn it into
    //(?=.*match1)(?=.*match2)(?!.*match3) - all escaped of course   	
	 QChar search_not_char = settings.value(SETTINGS_SEARCH_NOT_CHAR,DEFAULT_SEARCH_NOT_CHAR).toChar();
    
    QStringList words = filter.split(QRegularExpression("\\s+"));   
    QString regexpstring="(?=^.*$)"; // Seems a negative lookahead can't be first (!?), so this is a workaround
    #define START "(?=^.*"
    #define STARTN "(?!^.*"
    for(QString word:words){
        if(word.length()==0) break;
  			filterTextHasProject |= contexts.contains(word);

        if(word.at(0)==search_not_char){
        		regexpstring.append(STARTN);
           	regexpstring.append(QRegularExpression::escape(word.remove(0,1))+".*$)");
        }else{
	        regexpstring.append(START);
	        regexpstring.append(QRegularExpression::escape(word)+".*$)");
    	}
    }
   this->filterText.setPattern(regexpstring);	
//  	filterTextHasProject = filter.contains(QRegularExpression("(?:^|\\s+)\\+\\w*(?:\\s+|$)"));
  	
//  		qDebug()<<"todoProxyModel::updateFilterText: filterTextHasProject: "<<filterTextHasProject<<endline;
//  		qDebug()<<"   filter="<<filter<<endline;
	beginFilterChange();	
	
   this->setFilterRegularExpression(this->filterText);
   endFilterChange(QSortFilterProxyModel::Direction::Rows);
   refresh();
//   qDebug()<<"updateFilterText: filterTexthasProject="<<filterTextHasProject<<endline;
}
