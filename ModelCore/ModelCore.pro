#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:21
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

VERSION = 0.1.0
TARGET = ModelCore
TARGET_EXT = .dll # prevent version suffix on dll
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
    hsccoordinatepair.cpp \
    hmvariable.cpp \
    projectinputcsv.cpp \
    projectinputraster.cpp \
    projectinputvector.cpp \
    hsisimulation.cpp \
    hsccategorical.cpp \
    hscinflection.cpp

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
    hsccoordinatepair.h \
    hmvariable.h \
    projectinputcsv.h \
    projectinputraster.h \
    projectinputvector.h \
    hsisimulation.h \
    hsccategorical.h \
    hscinflection.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(release, debug|release): BUILD_TYPE = Release
else:CONFIG(debug, debug|release): BUILD_TYPE = Debug


win32 {
    ## There's some trickiness in windows 32 vs 64-bits
    !contains(QMAKE_TARGET.arch, x86_64) {
        ARCH = "32"
        message("x86 build (32 bit) ")
    } else {
        message("x86_64 build (64 bit) ")
        ARCH = "64"
    }

    # GDAL is required
    GDALWIN = $$PWD/../Libraries/gdalwin$$ARCH-1.10.1
    LIBS += -L$$GDALWIN/lib -lgdal_i
    INCLUDEPATH += $$GDALWIN/include
    DEPENDPATH += $$GDALWIN/include

}
macx{
    ## OSX common build here
    message("Mac OSX x86_64 build (64bit)")

    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE

    # GDAL is required
    GDALNIX = /Library/Frameworks/GDAL.framework/Versions/1.11/unix
    LIBS += -L$$GDALNIX/lib -lgdal
    INCLUDEPATH += $$GDALNIX/include
    DEPENDPATH  += $$GDALNIX/include
}

INCLUDEPATH += $$PWD/../../../GCD/gcd-console/GCDCore
DEPENDPATH += $$PWD/../../../GCD/gcd-console/GCDCore

INCLUDEPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
DEPENDPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager

LIBS += -L$$DESTDIR -lRasterManager
LIBS += -L$$DESTDIR -lGCDCore

message("Building to: $$DESTDIR")
