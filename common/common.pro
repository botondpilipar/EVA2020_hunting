! include(../common.pri)

TEMPLATE = lib
TARGET = common
DEFINES = COMMON

INCLUDEPATH += include
HEADERS += include/SingletonFactory.hpp \
            include/UnimplementedException.h \
            include/CachedRandomDevice.h

SOURCES += src/CachedRandomDevice.cpp
