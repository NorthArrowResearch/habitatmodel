#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:21
#
#-------------------------------------------------

QT       -= gui

TARGET = ModelCore
TEMPLATE = lib

DEFINES += MODELCORE_LIBRARY

SOURCES += model.cpp

HEADERS += model.h\
        modelcore_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
