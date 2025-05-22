#-------------------------------------------------
#
# Project created by QtCreator 2012-08-11T09:36:21
#
#-------------------------------------------------

QT       += core gui network widgets printsupport
QT +=xml statemachine

TARGET = Todour
TEMPLATE = app
#VERS = $$system(perl version.pl)
VERS = 2.99G
DEFINES += VER=\"\\\"$${VERS}\\\"\"
CONFIG += c++11
CONFIG+=sdk_no_version_check
CONFIG+=qt console
CONFIG+=debug
CONFIG+=fontAwesomeFree

macx{
ICON = icon.icns
#QMAKE_MAC_SDK = macosx11.0
}
win32 {
    RC_FILE = myresource.rc
}

include(QtAwesome/QtAwesome/QtAwesome.pri)
#include(UGlobalHotkey/uglobalhotkey.pri)

INCLUDEPATH += ./Caldav/

SOURCES += main.cpp\
    mainwindow.cpp \
    todotxt.cpp \
    todotablemodel.cpp \
    settingsdialog.cpp \
    aboutbox.cpp \
    quickadddialog.cpp \
    task.cpp \
    version.cpp \
    todo_undo.cpp \
    caldav.cpp \
	Caldav/CalendarClient_CalDAV.cpp \
	Caldav/CalendarEvent.cpp \
	Caldav/DateUtils.cpp \
	Caldav/CalendarClient_CalDAV_StateMachine.cpp    \
	Caldav/SimpleCrypt.cpp \
	Caldav/caldav_util.cpp
    

HEADERS  += mainwindow.h \
    todotxt.h \
    todotablemodel.h \
    settingsdialog.h \
    aboutbox.h \
    quickadddialog.h \
    def.h \
    task.h \
    version.h \
    todo_undo.h \
    todo_backend.h \
    caldav.h \
    Caldav/DateUtils.h \
    Caldav/SimpleCrypt.h \
    Caldav/CalendarEvent.h \
    Caldav/CalendarClient_CalDAV.h \
    Caldav/caldav_util.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    aboutbox.ui \
    quickadddialog.ui

OTHER_FILES += \
    version.pl \
    myresource.rc \
    icon.icsn \
    todour.ico \
    deploy.sh \
    todo.txt

DISTFILES += \
    Feature-move-show-all \
    Feature-AlwaysOnTop \
    Todour.desktop \
    autobuild/Todour.desktop \
    todour-latest.php \
    LICENSE \
    README \
    PRIVACY-POLICY \
    CHANGELOG \
    todour-latest_windows.php \
    todour-latest_mac.php \
    Feature_Url_Support \
    Feature-Advanced_filters.txt \
    Feature-follow_select \
    Feature-Enter_To_Search \
    Feature-customized-font-colors \
    Feature-Setup-ini \
    Feature-tagstrip \
    PLAN.md \
    todour.png \
    autobuild/package-windows.sh \
    autobuild/Setup.iss \
    autobuild/README.md \
    autobuild/build-todour-mac.pl \
    autobuild/todourdmg.json \
    autobuild/package-linux.sh \
    autobuild/DEBIAN_CONTROL_FILE.txt

RESOURCES += \
    resources.qrc

