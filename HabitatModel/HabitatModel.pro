#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:52
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10 #2

TARGET = HabitatModel
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    habitatmodelengine.cpp

HEADERS += \
    habitatmodelengine.h


win32 {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GCD/build-gcp-console-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Release/RasterManager/release/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GCD/build-gcp-console-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Debug/RasterManager/debug/ -lGCDCore

    CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GCD/build-gcp-console-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Release/RasterManager/release/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GCD/build-gcp-console-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Debug/RasterManager/debug/ -lGCDCore
}

unix{
    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Release/GCDCore/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/GCDCore/ -lGCDCore

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Release/RasterManager/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/RasterManager/ -lGCDCore
}

INCLUDEPATH += $$PWD/../../GCD/gcd-console/GCDCore
DEPENDPATH += $$PWD/../../GCD/gcd-console/GCDCore

INCLUDEPATH += $$PWD/../../GCD/gcd-console/RasterManager
DEPENDPATH += $$PWD/../../GCD/gcd-console/RasterManager

INCLUDEPATH += $$PWD/../ModelCore
DEPENDPATH += $$PWD/../ModelCore
