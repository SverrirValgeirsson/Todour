#-------------------------------------------------
#
# Project created by QtCreator 2012-08-11T09:36:21
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = Todour
TEMPLATE = app
VERS = $$system(perl version.pl)
DEFINES += VER=\"\\\"$${VERS}\\\"\"
CONFIG += c++11

macx{
ICON = icon.icns
QMAKE_MAC_SDK = macosx10.12
}
win32 {
    RC_FILE = myresource.rc
}

include(QtAwesome/QtAwesome/QtAwesome.pri)
include(UGlobalHotkey/uglobalhotkey.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    todotxt.cpp \
    todotablemodel.cpp \
    settingsdialog.cpp \
    aboutbox.cpp \
    quickadddialog.cpp

HEADERS  += mainwindow.h \
    todotxt.h \
    todotablemodel.h \
    settingsdialog.h \
    aboutbox.h \
    globals.h \
    quickadddialog.h \
    def.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    aboutbox.ui \
    quickadddialog.ui

OTHER_FILES += \
    version.pl \
    Setup.iss \
    myresource.rc \
    icon.icsn \
    newicon.ico \
    deploy.sh \
    todo.txt

DISTFILES += \
    todour-latest.php \
    make_standalone_windows.pl \
    LICENSE \
    README \
    PRIVACY-POLICY \
    make_standalone_linux.pl \
    todourdmg.json \
    CHANGELOG \
    todour-latest_windows.php \
    todour-latest_mac.php \
    Feature_Url_Support \
    Feature-Advanced_filters.txt \
    README.md \
    Feature-follow_select \
    Feature-Enter_To_Search \
    Feature-customized-font-colors \
    Feature-Setup-ini
