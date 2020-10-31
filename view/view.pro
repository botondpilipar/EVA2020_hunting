! include(../common.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    HuntingMainWindow.cpp \
    BoardGraphicUtility.cpp \
    FileChooserDialog.cpp

HEADERS += \
    BoardGraphicUtility.h \
    FileChooserDialog.h \
    HuntingMainWindow.h

FORMS += \
    FileChooserDialog.ui \
    HuntingMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../access/release/ -laccess
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../access/debug/ -laccess
else:unix: LIBS += -L$$OUT_PWD/../access/ -laccess

INCLUDEPATH += $$PWD/../access
DEPENDPATH += $$PWD/../access
