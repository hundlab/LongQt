#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LongQtGrapher
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

CONFIG += c++11

include(modellib/model.pri)
include(Grapher/Grapher.pri)

RESOURCES = LongQtGrapher.qrc 

linux {

    debug {
        QMAKE_CXXFLAGS += -g
        QMAKE_CXXFLAGS_RELEASE -= -O2
        TARGET =$$TARGET"-debug"
    }
    relase {
        QMAKE_CXXFLAGS += -static
        TARGET =$$TARGET"-relase"
    }
    profile {
        TARGET =$$TARGET"-profile"
    }

    TARGET = $$TARGET".out"
    DESTDIR = ./build 
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/obj
    RCC_DIR = ./build/obj
    UI_DIR = ./build/obj
}

SOURCES += LongQtGrapher.cpp

