#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T13:41:46
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = subdirs

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10 #2

SUBDIRS += \
    ModelCore \
    HabitatModel

win32 {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../gcd-cpp-Qt_5_3_clang_64_release/RasterManager/release/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../gcd-cpp-Qt_5_3_clang_64_release/RasterManager/debug/ -lGCDCore

    CONFIG(release, debug|release): LIBS += -L$$PWD/../gcd-cpp-Qt_5_3_clang_64_release/RasterManager/release/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../gcd-cpp-Qt_5_3_clang_64_release/RasterManager/debug/ -lGCDCore
}

unix{
    LIBS += -L$$PWD/../gcd-cpp-Qt_5_3_clang_64_release/GCDCore/ -lGCDCore
}

INCLUDEPATH += $$PWD/../gcd-cpp/GCDCore
DEPENDPATH += $$PWD/../gcd-cpp/GCDCore

INCLUDEPATH += $$PWD/../gcd-cpp/RasterManager
DEPENDPATH += $$PWD/../gcd-cpp/RasterManager
