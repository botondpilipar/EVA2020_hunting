! include(../common.pri)

QT += core

TEMPLATE = lib
TARGET = access
DEFINES = ACCESS

INCLUDEPATH += include
HEADERS += include/ISerializable.h \
    include/IDataInitializable.hpp \
    include/IDeserializable.h \
    include/StreamSerializationSource.h \
    include/StreamSerializationTarget.h \
            include/ISerializationSource.h \
            include/ISerializationTarget.h \
            include/DeserializationFailedException.h \
            include/QDataStreamSerializable.h \
            include/Serializer.h \
            include/FileException.h


SOURCES += \
    src/StreamSerializationSource.cpp \
    src/StreamSerializationTarget.cpp \
    src/Serializer.cpp

PRECOMPILED_HEADER = include/access_pch.h
