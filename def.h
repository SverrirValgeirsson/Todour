#ifndef DEF_H
#define DEF_H
#include <QSettings>

/* Workaround for the Qt::endl not existing before 5.14.something */
#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define endline endl
#else
#define endline Qt::endl
#endif


// Names of settings in QSettings

//******************* General Options ***********************
// start in tray mode. not tested on wayland.
#define SETTINGS_TRAY_ENABLED "tray_enabled"
#define DEFAULT_TRAY_ENABLED false

// Is global hotkey active?
#define SETTINGS_HOTKEY_ENABLE "hotkey_enable"
#define DEFAULT_HOTKEY_ENABLE false

// select the hotkey for adding a task even when todour is iconised. Doesn't work on Wayland yet.
#define SETTINGS_HOTKEY "hotkey"
#define DEFAULT_HOTKEY "Ctrl+Alt+t"

// Show the input-dates / close dates in the tasks.
#define SETTINGS_SHOW_DATES "show_dates"
#define DEFAULT_SHOW_DATES false

// move deleted tasks to deleted.txt or not?   Not used today
#define SETTINGS_DELETED_FILE "deleted_file"
#define DEFAULT_DELETED_FILE false

// save the search-string to show the same when restarting the app.
//#define SETTINGS_SEARCH_STRING "search_string"
//#define DEFAULT_SEARCH_STRING ""

// Monitor todo.txt file for change and auto-reload. working ???
#define SETTINGS_AUTOREFRESH "autorefresh"
#define DEFAULT_AUTOREFRESH true

// state of the window.
#define SETTINGS_GEOMETRY "geometry"

// ???
#define SETTINGS_SAVESTATE "savestate"

// state of the window.
#define SETTINGS_MAXIMIZED "maximized"

// Window state
#define SETTINGS_STAY_ON_TOP "stay_on_top"
#define DEFAULT_STAY_ON_TOP false

// Position of the splitter
#define SETTING_SPLITTER_SIZE "splitterSizes"

// Show the note.txt editing box
#define SETTINGS_NOTE_ENABLE "note_enable"
#define DEFAULT_NOTE_ENABLE true

// todo.txt directory location
#define SETTINGS_DIRECTORY "directory"
#define DEFAULT_DIRECTORY ""

// keywords for inactive tasks
#define SETTINGS_INACTIVE "inactive"
#define DEFAULT_INACTIVE "LATER:;WAIT:"

// Respect due or not?   If yes, color the overdue and the "nearly overdue" (reminder).
#define SETTINGS_DUE "due"
#define DEFAULT_DUE false

// Days delay before the due date to be "nearly overdue" (reminder)
#define SETTINGS_DUE_WARNING "due_warning"
#define DEFAULT_DUE_WARNING 3

// Context lock activated, include current context in new tasks.
#define SETTINGS_CONTEXT_LOCK "context_lock"
#define DEFAULT_CONTEXT_LOCK false

// auto check for update at startup. Still connected to Nerdur...
#define SETTINGS_CHECK_UPDATES "check_updates"
#define DEFAULT_CHECK_UPDATES false

// ???
#define SETTINGS_LAST_UPDATE_CHECK "last_update_check"

// Hide duplicate tasks (not working now, issue with "duplicate")
#define SETTINGS_REMOVE_DOUBLETS "remove_doublets"
#define DEFAULT_REMOVE_DOUBLETS false

// ??
#define SETTINGS_UUID "uuid"
#define DEFAULT_UUID "0000-0000-0000-0000"

// QVariant of Business days. Can we improve?
#define SETTINGS_BUSINESS_DAYS "business_days"
#define DEFAULT_BUSINESS_DAYS_FIRST 1
#define DEFAULT_BUSINESS_DAYS_LAST 5



//***************** Fonts & Colors ***************
// "nearly overdue" (reminder) color  (orange)
#define SETTINGS_DUE_WARNING_COLOR "due_warning_color"
#define DEFAULT_DUE_WARNING_COLOR 0xFFFFA500

// overdue color (red)
#define SETTINGS_DUE_LATE_COLOR "due_late_color"
#define DEFAULT_DUE_LATE_COLOR 0xFFFF0000

// Active task font
#define SETTINGS_ACTIVE_FONT "activefont"
#define DEFAULT_ACTIVE_FONT "Noto Sans,11,-1,5,400,0,0,0,0,0,0,0,0,0,0,1,,0,0"

// Active task font color
#define SETTINGS_ACTIVE_COLOR "activecolor"
#define DEFAULT_ACTIVE_COLOR 0xFF000000

//Inactive task font
#define SETTINGS_INACTIVE_FONT "inactivefont"
#define DEFAULT_INACTIVE_FONT "Noto Sans,10,-1,5,400,0,0,0,0,0,0,0,0,0,0,1,Regular,0,0"

//inactive task font color
#define SETTINGS_INACTIVE_COLOR "inactivecolor"
#define DEFAULT_INACTIVE_COLOR 0xFF555555




//************ FILTERING ***************
// Live search not activated : need to press ENTER to filter.
#define SETTINGS_LIVE_SEARCH "liveSearch"
#define DEFAULT_LIVE_SEARCH true

// "not" char for search. Normally '!'
#define SETTINGS_SEARCH_NOT_CHAR "search_not_char"
#define DEFAULT_SEARCH_NOT_CHAR '!'

// Hide task with due date before duedate
#define SETTINGS_DUE_AS_THRESHOLD "due_as_threshold"
#define DEFAULT_DUE_AS_THRESHOLD false

// Maximum no. of tasks to show in "today's view". not working yet.
#define SETTINGS_NN_TODAY "nn_today"
#define DEFAULT_NN_TODAY 5

// respect threshold. to be deleted.
//#define SETTINGS_THRESHOLD "threshold"
//#define DEFAULT_THRESHOLD false

// respect date threshold
#define SETTINGS_THRESHOLD_DATES "threshold_dates"
#define DEFAULT_THRESHOLD_DATES true

//respect context / project threshold
#define SETTINGS_THRESHOLD_LABELS "threshold_labels"
#define DEFAULT_THRESHOLD_LABELS false

// consider threshold as inactive. Otherwise, just hide them.   to be removed.
#define SETTINGS_THRESHOLD_INACTIVE "threshold_inactive"
#define DEFAULT_THRESHOLD_INACTIVE false

// activate the "enhanced PM" mode, hide all the project-assigned tasks (+...) except if selected in filter. 
#define SETTINGS_ENHANCED_PM "enhanced_pm"
#define DEFAULT_ENHANCED_PM true

// Filter selected to show all tasks.
#define SETTINGS_SHOW_ALL "show_all"
#define DEFAULT_SHOW_ALL false

// hide inactive task or not.
#define SETTINGS_HIDE_INACTIVE "hideinactive"
#define DEFAULT_HIDE_INACTIVE false

#define SETTINGS_TODAYS_VIEW "todaysview"
#define DEFAULT_TODAYS_VIEW false

#define SETTINGS_PROJECT_VIEW "projectview"
#define DEFAULT_PROJECT_VIEW false



//*********** SORTING *****************
// activate the alphabetical sorting
#define SETTINGS_SORT_AZ "sort_az"
#define DEFAULT_SORT_AZ true

//activate the sort by input date
#define SETTINGS_SORT_IDATE "sort_idate"
#define DEFAULT_SORT_IDATE false

// sort-mode alphabetically the tasks (historical todo.txt)
#define SETTINGS_SORT_ALPHA "sort_alpha"
#define DEFAULT_SORT_ALPHA true

// sort inactive tasks last or integrate in the sort order?
#define SETTINGS_SEPARATE_INACTIVES "separateinactive"
#define DEFAULT_SEPARATE_INACTIVES true



//*********** CREATING & CLOSING ***********
// priority when none is entered. Can be void
#define SETTINGS_DEFAULT_PRIORITY "default_priority"
#define DEFAULT_DEFAULT_PRIORITY "B"

// What to do with priority on task done? According to todo.txt rules, it should be deleted or...
#define SETTINGS_PRIO_ON_CLOSE "prio_on_close"
#define DEFAULT_PRIO_ON_CLOSE 0

// Add dates to tasks (creation date = input date) close date
#define SETTINGS_DATES "dates"
#define DEFAULT_DATES false

// default value to add to a rec task when this is done. if task has no t: nor due:, that one will be added.
#define SETTINGS_DEFAULT_THRESHOLD "default_threshold"
#define DEFAULT_DEFAULT_THRESHOLD "due:"










#define SETTINGS_BACKEND "backend"
#define SETTINGS_CALDAV_URL "caldav_url"
#define SETTINGS_CALDAV_USERNAME "caldav_username"
#define SETTINGS_CALDAV_PASSWORD "caldav_password"


enum prio_on_close {removeit=0,moveit,tagit};

#endif // DEF_H
