! include (../test.pri)


QT += testlib
QT += core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = board_test
TEMPLATE = app

SOURCES +=


unix|win32: LIBS += -L$$OUT_PWD/../../access/ -laccess

INCLUDEPATH += $$PWD/../../access
DEPENDPATH += $$PWD/../../access

unix|win32: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model

SOURCES += \
    ModelTest.cpp
