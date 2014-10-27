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
    hsccategory.h \
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

        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release32 -lRasterManager
        else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug32 -lRasterManager

        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release32 -lGCDCore
        else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug32 -lGCDCore

    } else {
        message("x86_64 build")

        # Locate GDAL
        LIBS += -L$$PWD/../Libraries/gdalwin64-1.10.1/lib/ -lgdal_i
        INCLUDEPATH += $$PWD/../Libraries/gdalwin64-1.10.1/include
        DEPENDPATH += $$PWD/../Libraries/gdalwin64-1.10.1/include

        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release64 -lRasterManager
        else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug64 -lRasterManager

        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release64 -lGCDCore
        else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug64 -lGCDCore
    }
}
unix{

    # Compile to a central location
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Release
    else:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../../../Deploy/Debug

    LIBS += -L/Library/Frameworks/GDAL.framework/Versions/1.11/unix/lib -lgdal
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Versions/1.11/unix/include
    DEPENDPATH  += /Library/Frameworks/GDAL.framework/Versions/1.11/unix/include

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release -lRasterManager
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug -lRasterManager

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Release -lGCDCore
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Deploy/Debug -lGCDCore
}

INCLUDEPATH += $$PWD/../../../GCD/gcd-console/GCDCore
DEPENDPATH += $$PWD/../../../GCD/gcd-console/GCDCore

INCLUDEPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
DEPENDPATH += $$PWD/../../../RasterManager/rastermanager/RasterManager
