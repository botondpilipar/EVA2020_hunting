! include(../common.pri)

QT += core

TEMPLATE = lib
TARGET = logic
DEFINES = LOGIC

HEADERS += \
    include/BoardUtility.h \
    include/HuntingBoard.h \
    include/HuntingBoardData.h \
    include/HuntingTypes.h \
    include/IGameActor.hpp \
    include/IGameBoard.h \
    include/IGameControlUnit.h \
    include/IMovableObject.hpp \
    include/ISettingsChangedObserver.h \
    include/ModelTypes.hh \
    include/model_pch.h

SOURCES += \
    src/BoardUtility.cpp \
    src/HuntingBoard.cpp \
    src/HuntingTypes.cpp
