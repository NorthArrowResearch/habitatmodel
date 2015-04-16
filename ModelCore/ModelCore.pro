#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:21
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

VERSION = 1.0.3
TARGET = ModelCore
TARGET_EXT = .dll # prevent version suffix on dll
TEMPLATE = lib

CONFIG += c++11

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
    xmlfile.cpp

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
    habitat_exception.h


CONFIG(release, debug|release): BUILD_TYPE = release
else:CONFIG(debug, debug|release): BUILD_TYPE = debug

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

    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE$$ARCH
}
macx{
    ## OSX common build here
    message("Mac OSX x86_64 build (64bit)")

    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

    # GDAL is required
    GDALNIX = /Library/Frameworks/GDAL.framework/Versions/1.11/unix
    LIBS += -L$$GDALNIX/lib -lgdal
    INCLUDEPATH += $$GDALNIX/include
    DEPENDPATH  += $$GDALNIX/include
}
unix:!macx {
    message("Unix")
    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE

    target.path = /usr/lib
    INSTALLS += target

    # GDAL is required
    LIBS += -L/usr/lib -lgdal
    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH  += /usr/include/gdal
}

INCLUDEPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
message($$INCLUDEPATH)
DEPENDPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager

LIBS += -L$$DESTDIR -lRasterManager

message("Building to: $$DESTDIR")
