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
CONFIG+=sdk_no_version_check

macx{
ICON = icon.icns
#QMAKE_MAC_SDK = macosx11.0
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
    myresource.rc \
    icon.icsn \
    newicon.ico \
    deploy.sh \
    todo.txt

DISTFILES += \
    Feature-move-show-all \
    Feature-AlwaysOnTop \
    Todour.desktop \
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
    newicon.png \
    autobuild/package-windows.sh \
    autobuild/Setup.iss \
    autobuild/README.md \
    autobuild/build-todour-mac.pl \
    autobuild/todourdmg.json \
    autobuild/package-linux.sh \
    autobuild/DEBIAN_CONTROL_FILE.txt

RESOURCES += \
    resources.qrc

