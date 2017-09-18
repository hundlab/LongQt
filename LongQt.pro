#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LongQt
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.12

CONFIG += c++11 wayland-compositor

include(GridEditor/GridEdit.pri)
include(LongQtMain/LongQtMain.pri)
include(modellib/model.pri)
include(Grapher/Grapher.pri)

RESOURCES = LongQt.qrc

linux {
    debug {
        QMAKE_CXXFLAGS += -g -O0
        QMAKE_CXXFLAGS_RELEASE -= -O2
    }
    relase {
        QMAKE_CXXFLAGS += -static
    }
    TARGET = $$TARGET".out"

    DESTDIR = ./build
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/obj
    RCC_DIR = ./build/obj
    UI_DIR = ./build/obj
}

SOURCES += LongQt.cpp

