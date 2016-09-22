#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:21
#
#-------------------------------------------------

QT       += core xml
QT       -= gui widgets

VERSION = 1.4.0
TARGET = ModelCore
TARGET_EXT = .dll # prevent version suffix on dll
TEMPLATE = lib

DEFINES += HMLIBVERSION=\\\"$$VERSION\\\" # Makes verion available to c++
DEFINES += MODELCORE_LIBRARY

SOURCES += \
    simulation.cpp \
    hsi.cpp \
    hsicurve.cpp \
    hsc.cpp \
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
    hscinflection.cpp \
    hsccategory.cpp \
    habitat_interface.cpp \
    xmlfile.cpp \
    fissimulation.cpp \
    simulationfisinput.cpp \
    fiscore.cpp

HEADERS +=\
    simulation.h \
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
    hscinflection.h \
    hsccategory.h \
    habitat_interface.h \
    xmlfile.h \
    habitat_exception.h \
    fissimulation.h \
    simulationfisinput.h \
    fiscore.h \
    habitat_misc.h \
    benchmark.h

CONFIG(release, debug|release): BUILD_TYPE = release
else:CONFIG(debug, debug|release): BUILD_TYPE = debug

GDALLIB = $$(GDALLIBDIR)

RASTERMAN = $$(RASTERMANDIR)
isEmpty(RASTERMAN){
    RASTERMAN= $$PWD/../../rasterman
    message("RASTERMANDIR not set. Defaulting to $$RASTERMAN")
}else{
    RASTERMAN=$$(RASTERMANDIR)
    message("RASTERMANDIR set to $$RASTERMAN")
}

INCLUDEPATH += $$RASTERMAN/RasterManager
DEPENDPATH += $$RASTERMAN/RasterManager


win32 {
    isEmpty(GDALLIB){
        error("GDALLIBDIR not set. This will cause failures")
    }

    ## There's some trickiness in windows 32 vs 64-bits
    !contains(QMAKE_TARGET.arch, x86_64) {
        ARCH = "32"
        message("x86 build (32 bit) ")
    } else {
        message("x86_64 build (64 bit) ")
        ARCH = "64"
    }

    # GDAL is required
    LIBS += -L$$GDALLIB/lib -lgdal_i
    INCLUDEPATH += $$GDALLIB/include
    DEPENDPATH += $$GDALLIB/include

    DESTDIR = $$OUT_PWD/../../deploy/$$TOOLDIR$$BUILD_TYPE$$ARCH
    LIBS += -L$$DESTDIR -lRasterManager

}
macx{
    ## OSX common build here
    message("Mac OSX x86_64 build (64bit)")
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11 #2 ElCapitan
    QMAKE_MAC_SDK = macosx10.11

    isEmpty(GDALLIB){
        warning("GDALLIBDIR not set. Defaulting to /usr/local")
        GDALLIB = /usr/local
    }

    # This is mostly to keep the debug builds sane
    DESTDIR = $$OUT_PWD/../../deploy/$$BUILD_TYPE$$ARCH

    # GDAL is required
    LIBS += -L$$GDALLIB/lib -lgdal
    INCLUDEPATH += $$GDALLIB/include
    DEPENDPATH  += $$GDALLIB/include

    LIBS += -L$$DESTDIR -lRasterManager

    # Where are we installing to
    target.path = /usr/local/lib
    INSTALLS += target

}
linux {
    isEmpty(GDALLIB){
        warning("GDALLIBDIR not set. Defaulting to /usr/local")
        GDALLIB = /usr/local
    }

    # GDAL is required
    LIBS += -L$$GDALLIB/lib -lgdal
    INCLUDEPATH += $$GDALLIB/include
    DEPENDPATH  += $$GDALLIB/include

    # Where are we installing to
    target.path = /usr/local/lib
    INSTALLS += target
}
