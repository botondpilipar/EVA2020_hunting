WARNINGS += -Wall
CONFIG += c++17
CONFIG += warn_on
QMAKE_CXXFLAGS += -v
QMAKE_CXXFLAGS += -std=c++1z

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

INCLUDEPATH += $$top_srcdir/access/include
INCLUDEPATH += $$top_srcdir/common/include
INCLUDEPATH += $$top_srcdir/logic/include
INCLUDEPATH += $$top_srcdir/model/include

PRECOMPILED_HEADER = $$top_srcdir/common/include/common_pch.h



