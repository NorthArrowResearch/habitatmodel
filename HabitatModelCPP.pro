#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T13:41:46
#
#-------------------------------------------------

QT       -= core gui
CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    ModelCore \
    HabitatModel

win32{
  message(Building fow Win32)
}
win64{
  message(Building fow Win64)
}
macx{
  message(Building fow macx)
}
linux{
  message(Building fow linux)
}