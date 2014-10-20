TEMPLATE = subdirs

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
