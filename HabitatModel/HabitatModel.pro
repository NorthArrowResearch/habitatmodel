#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T15:37:52
#
#-------------------------------------------------

QT       += core
QT       += xml

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

    ## Windows common build here
    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

        # Compile to a central location
        CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Release32
        else:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Debug32

        LIBS += -L$$PWD/../Libraries/gdalwin32-1.10.1/lib/ -lgdal_i
        INCLUDEPATH += $$PWD/../Libraries/gdalwin32-1.10.1/include
        DEPENDPATH += $$PWD/../Libraries/gdalwin32-1.10.1/include

    } else {
        message("x86_64 build")

        # Locate GDAL
        LIBS += -L$$PWD/../Libraries/gdalwin64-1.10.1/lib/ -lgdal_i
        INCLUDEPATH += $$PWD/../Libraries/gdalwin64-1.10.1/include
        DEPENDPATH += $$PWD/../Libraries/gdalwin64-1.10.1/include
    }
}

unix{

    # Compile to a central location
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Release
    else:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Debug

    LIBS += -L/Library/Frameworks/GDAL.framework/Versions/1.11/unix/lib -lgdal
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Versions/1.11/unix/include
    DEPENDPATH  += /Library/Frameworks/GDAL.framework/Versions/1.11/unix/include

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release -lModelCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug -lModelCore

}

LIBS += -L$$OUT_PWD/../ModelCore/ -lModelCore


INCLUDEPATH += $$PWD/../../../GCD/gcd-console/GCDCore
DEPENDPATH += $$PWD/../../../GCD/gcd-console/GCDCore

INCLUDEPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
DEPENDPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager

INCLUDEPATH += $$PWD/../ModelCore
DEPENDPATH += $$PWD/../ModelCore
