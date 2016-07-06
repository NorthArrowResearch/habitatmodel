#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:52
#
#-------------------------------------------------

QT       += core xml
QT       -= gui

VERSION = 1.4.0
DEFINES += VERSION=\\\"$$VERSION\\\" # Makes verion available to c++

TARGET = HabitatModel
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp \
    habitatmodelengine.cpp

HEADERS += \
    habitatmodelengine.h

INCLUDEPATH += $$PWD/../ModelCore
DEPENDPATH += $$PWD/../ModelCore

CONFIG(release, debug|release): BUILD_TYPE = release
else:CONFIG(debug, debug|release): BUILD_TYPE = debug

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
INCLUDEPATH += $$PWD/../ModelCore
DEPENDPATH += $$PWD/../ModelCore

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
    GDALWIN = $$(GDALLIBDIR)
    LIBS += -L$$GDALWIN/lib -lgdal_i
    INCLUDEPATH += $$GDALWIN/include
    DEPENDPATH += $$GDALWIN/include

    DESTDIR = $$OUT_PWD/../../../deploy/$$TOOLDIR$$BUILD_TYPE$$ARCH

    # Compile to a central location
    LIBS += -L$$DESTDIR -lModelCore
    LIBS += -L$$DESTDIR -lRasterManager

}
macx{
    ## OSX common build here
    message("Mac OSX x86_64 build (64bit)")
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
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

    LIBS += -L$$DESTDIR -lModelCore
    LIBS += -L$$DESTDIR -lRasterManager

    # Where are we installing to
    target.path = /usr/local/bin
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

    LIBS += -L$$OUT_PWD/../ModelCore -lModelCore
    LIBS += -L/usr/local/lib -lRasterManager

    target.path = /usr/local/bin
    INSTALLS += target
}
