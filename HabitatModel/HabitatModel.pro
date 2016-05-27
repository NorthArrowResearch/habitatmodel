#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:52
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

VERSION = 1.2.0
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
    RASTERMAN= $$PWD/../../../RasterManager/rastermanager/
    message("RASTERMANDIR not set. Defaulting to $$RASTERMAN")
}else{
    RASTERMAN=$$(RASTERMANDIR)
    message("RASTERMANDIR set to $$RASTERMAN")
}

INCLUDEPATH += $$RASTERMAN/RasterManager
DEPENDPATH += $$RASTERMAN/RasterManager

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

    DESTDIR = $$OUT_PWD/../../../Deploy/$$TOOLDIR$$BUILD_TYPE$$ARCH

    # Compile to a central location
    LIBS += -L$$DESTDIR -lModelCore
    LIBS += -L$$DESTDIR -lRasterManager

}
macx{
    ## OSX common build here
    message("Mac OSX x86_64 build (64bit)")
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
    QMAKE_MAC_SDK = macosx10.11

    # Compile to a central location
    DESTDIR = $$OUT_PWD/../../../Deploy/$$BUILD_TYPE

    # GDAL is required
    GDALNIX = /Library/Frameworks/GDAL.framework/Versions/1.11/unix
    LIBS += -L$$GDALNIX/lib -lgdal
    INCLUDEPATH += $$GDALNIX/include
    DEPENDPATH  += $$GDALNIX/include

    LIBS += -L$$OUT_PWD/../ModelCore -lModelCore
    LIBS += -L$$DESTDIR -lRasterManager
}
linux {
    GDALNIX = /Library/Frameworks/GDAL.framework/Versions/1.11/unix

    # GDAL is required
    LIBS += -L/usr/lib -lgdal
    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH  += /usr/include/gdal

    target.path = /usr/bin
    INSTALLS += target

    LIBS += -L$$OUT_PWD/../ModelCore -lModelCore
    LIBS += -L/usr/lib -lRasterManager

}
