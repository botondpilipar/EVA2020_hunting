! include(../common.pri)
! include (test.pri)

TEMPLATE = subdirs
TARGET = test

SUBDIRS += access \
           model \
           utility
