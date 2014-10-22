#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:21
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

TARGET = ModelCore
TEMPLATE = lib

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10 #2

DEFINES += MODELCORE_LIBRARY

SOURCES += model.cpp \
    simulation.cpp \
    fileinput.cpp \
    rasterinput.cpp \
    vectorinput.cpp \
    hsi.cpp \
    hsicurve.cpp \
    hsc.cpp \
    namedobject.cpp \
    project.cpp \
    projectinput.cpp \
    simulationhscinput.cpp \
    unit.cpp \
    hsccategory.cpp \
    hsccoordinatepair.cpp \
    hmvariable.cpp \
    modelengine.cpp \
    projectinputcsv.cpp \
    projectinputraster.cpp \
    projectinputvector.cpp \
    hsisimulation.cpp

HEADERS += model.h\
        modelcore_global.h \
    simulation.h \
    fileinput.h \
    rasterinput.h \
    vectorinput.h \
    hsi.h \
    hsicurve.h \
    hsc.h \
    namedobject.h \
    habitatmodel_global.h \
    project.h \
    projectinput.h \
    simulationhscinput.h \
    unit.h \
    hsccategory.h \
    hsccoordinatepair.h \
    hmvariable.h \
    modelengine.h \
    projectinputcsv.h \
    projectinputraster.h \
    projectinputvector.h \
    hsisimulation.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-GCDCPP-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Release/RasterManager/release/ -lRasterManager
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-GCDCPP-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Debug/RasterManager/debug/ -lRasterManager

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-GCDCPP-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Release/GCDCore/release/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-GCDCPP-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Debug/GCDCore/debug/ -lGCDCore
}
unix{
    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Release/GCDCore/ -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/GCDCore/ -lGCDCore

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Release/RasterManager/ -lRasterManager
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/RasterManager/ -lRasterManager
}

INCLUDEPATH += $$PWD/../../../GCD/gcd-console/GCDCore
DEPENDPATH += $$PWD/../../../GCD/gcd-console/GCDCore

INCLUDEPATH += $$PWD/../../../GCD/gcd-console/RasterManager
DEPENDPATH += $$PWD/../../../GCD/gcd-console/RasterManager
