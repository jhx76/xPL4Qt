#-------------------------------------------------
#
# Project created by QtCreator 2014-06-29T20:18:36
#
#-------------------------------------------------

QT       -= gui

TARGET = xPL4Qt
TEMPLATE = lib
CONFIG += staticlib

SOURCES += xPL4Qt.cpp

HEADERS += xPL4Qt.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
