! include (../test.pri)


QT += testlib
QT += core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = board_test
TEMPLATE = app

SOURCES += \
    AccessTest.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../access/release/ -laccess
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../access/debug/ -laccess
else:unix: LIBS += -L$$OUT_PWD/../../access/ -laccess

INCLUDEPATH += $$PWD/../../access
DEPENDPATH += $$PWD/../../access
