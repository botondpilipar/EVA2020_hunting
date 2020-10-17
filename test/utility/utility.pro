! include (../test.pri)


QT += testlib
QT += core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = board_test
TEMPLATE = app

SOURCES += main.cpp
