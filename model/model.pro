! include(../common.pri)

QT += core

TEMPLATE = lib
TARGET = logic
DEFINES = LOGIC

HEADERS += \
    include/BridgeCrossingBoard.h \
    include/BridgeCrossingBoardData.h \
    include/BridgeCrossingPlayer.h \
    include/BridgeCrossingPlayerData.h \
    include/BridgeCrossingSettings.h \
    include/BridgeCrossingTypes.h \
    include/IGameActor.hpp \
    include/IGameBoard.h \
    include/IGameControlUnit.h \
    include/IMovableObject.hpp \
    include/ISettingsChangedObserver.h \
    include/model_pch.h

SOURCES += \
    src/BridgeCrossingBoard.cpp \
    src/BridgeCrossingBoardData.cpp \
    src/BridgeCrossingPlayer.cpp \
    src/BridgeCrossingPlayerData.cpp \
    src/BridgeCrosssingSettings.cpp
