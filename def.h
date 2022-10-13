#ifndef DEF_H
#define DEF_H

/* Workaround for the Qt::endl not existing before 5.14.something */
#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define endline endl
#else
#define endline Qt::endl
#endif



// Defaults and definitions for settings
#define DEFAULT_HOTKEY "Ctrl+Alt+t"
#define DEFAULT_HOTKEY_ENABLE false
#define DEFAULT_SHOW_DATES false
#define DEFAULT_DELETED_FILE false
#define DEFAULT_SEARCH_STRING ""
#define DEFAULT_THRESHOLD false
#define DEFAULT_THRESHOLD_LABELS false
#define DEFAULT_THRESHOLD_INACTIVE false
#define DEFAULT_AUTOREFRESH true
#define DEFAULT_LIVE_SEARCH true
#define DEFAULT_DIRECTORY ""
#define DEFAULT_INACTIVE "LATER:;WAIT:"
#define DEFAULT_SEPARATE_INACTIVES false
#define DEFAULT_ACTIVE_COLOR 0xFF000000
#define DEFAULT_INACTIVE_COLOR 0xFF555555
#define DEFAULT_DATES false
#define DEFAULT_CONTEXT_LOCK false
#define DEFAULT_TRAY_ENABLED false
#define DEFAULT_SHOW_ALL false
#define DEFAULT_DUE false
#define DEFAULT_DUE_WARNING 3
#define DEFAULT_DUE_WARNING_COLOR 0xFFFFA500
#define DEFAULT_DUE_LATE_COLOR 0xFFFF0000
#define DEFAULT_CHECK_UPDATES true
#define DEFAULT_PRIO_ON_CLOSE 0
#define DEFAULT_REMOVE_DOUBLETS false
#define DEFAULT_UUID "0000-0000-0000-0000"
#define DEFAULT_STAY_ON_TOP false
#define DEFAULT_SEARCH_NOT_CHAR '!'
#define DEFAULT_DEFAULT_THRESHOLD "due:"
#define DEFAULT_BUSINESS_DAYS_FIRST 1
#define DEFAULT_BUSINESS_DAYS_LAST 5



// Names of settings in QSettings
#define SETTINGS_DUE "due"
#define SETTINGS_DUE_WARNING "due_warning"
#define SETTINGS_DUE_WARNING_COLOR "due_warning_color"
#define SETTINGS_DUE_LATE_COLOR "due_late_color"
#define SETTINGS_TRAY_ENABLED "tray_enabled"
#define SETTINGS_HOTKEY "hotkey"
#define SETTINGS_SHOW_ALL "show_all"
#define SETTINGS_HOTKEY_ENABLE "hotkey_enable"
#define SETTINGS_SHOW_DATES "show_dates"
#define SETTINGS_DELETED_FILE "deleted_file"
#define SETTINGS_SEARCH_STRING "search_string"
#define SETTINGS_THRESHOLD "threshold"
#define SETTINGS_THRESHOLD_LABELS "threshold_labels"
#define SETTINGS_THRESHOLD_INACTIVE "threshold_inactive"
#define SETTINGS_AUTOREFRESH "autorefresh"
#define SETTINGS_GEOMETRY "geometry"
#define SETTINGS_SAVESTATE "savestate"
#define SETTINGS_MAXIMIZED "maximized"
#define SETTINGS_LIVE_SEARCH "liveSearch"
#define SETTINGS_SORT_ALPHA "sort_alpha"
#define SETTINGS_DIRECTORY "directory"
#define SETTINGS_INACTIVE "inactive"
#define SETTINGS_SEPARATE_INACTIVES "separateinactive"
#define SETTINGS_ACTIVE_COLOR "activecolor"
#define SETTINGS_INACTIVE_COLOR "inactivecolor"
#define SETTINGS_ACTIVE_FONT "activefont"
#define SETTINGS_INACTIVE_FONT "inactivefont"
#define SETTINGS_DATES "dates"
#define SETTINGS_CONTEXT_LOCK "context_lock"
#define SETTINGS_CHECK_UPDATES "check_updates"
#define SETTINGS_LAST_UPDATE_CHECK "last_update_check"
#define SETTINGS_PRIO_ON_CLOSE "prio_on_close"
#define SETTINGS_FONT_SIZE "font_size"
#define SETTINGS_REMOVE_DOUBLETS "remove_doublets"
#define SETTINGS_UUID "uuid"
#define SETTINGS_STAY_ON_TOP "stay_on_top"
#define SETTINGS_SEARCH_NOT_CHAR "search_not_char"
#define SETTINGS_DEFAULT_THRESHOLD "default_threshold"
#define SETTINGS_BUSINESS_DAYS "business_days"

enum prio_on_close {removeit=0,moveit,tagit};

#endif // DEF_H
