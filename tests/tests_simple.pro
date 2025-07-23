QT += testlib core
TARGET = todour_core_tests
CONFIG += testcase console
CONFIG -= app_bundle

# Add version definition for tests
DEFINES += VER=\\\"test_version\\\"

# Test core classes only (without complex UI dependencies)
SOURCES += \
    ../todotxt.cpp \
    test_main_simple.cpp \
    test_todotxt.cpp

HEADERS += \
    ../todotxt.h \
    ../globals.h \
    ../def.h \
    test_todotxt.h

# Test data directory
DEFINES += TEST_DATA_DIR=\\\"$$PWD/data/\\\"

OTHER_FILES += \
    data/test_todo.txt \
    data/test_done.txt \
    data/empty_todo.txt
