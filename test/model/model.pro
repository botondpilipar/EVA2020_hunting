! include (../test.pri)


QT += testlib
QT += core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TARGET = board_test
TEMPLATE = app

SOURCES += \
    ModelTest.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../access/release/ -laccess
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../access/debug/ -laccess
else:unix: LIBS += -L$$OUT_PWD/../../access/ -laccess

INCLUDEPATH += $$PWD/../../access
DEPENDPATH += $$PWD/../../access
