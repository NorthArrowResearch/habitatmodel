#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:52
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

VERSION = 0.1.0
DEFINES += VERSION=\\\"$$VERSION\\\" # Makes verion available to c++

TARGET = HabitatModel
TEMPLATE = app

CONFIG += c++11

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp \
    habitatmodelengine.cpp

HEADERS += \
    habitatmodelengine.h


CONFIG(release, debug|release): BUILD_TYPE = release
else:CONFIG(debug, debug|release): BUILD_TYPE = debug

INCLUDEPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
DEPENDPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager

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
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE

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

    # GDAL is required
    LIBS += -L/usr/lib -lgdal
    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH  += /usr/include/gdal
}

LIBS += -L$$DESTDIR -lModelCore
LIBS += -L$$DESTDIR -lRasterManager

message("Building to: $$DESTDIR")
