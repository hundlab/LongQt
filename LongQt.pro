#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LongQt
TEMPLATE = app

CONFIG += c++17 wayland-compositor static

include(src/GridEditor/GridEdit.pri)
include(src/LongQtMain/LongQtMain.pri)
include(../LongQt-model/src/model.pri)
include(src/Grapher/Grapher.pri)
include(src/Utils.pri)

RESOURCES += LongQt.qrc

windows {
    RC_FILE = LongQt.rc
}
macx {
    QMAKE_MAC_SDK = macosx10.12
    ICON = LongQt.icns
}
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

#QMAKE_CXXFLAGS += -P
SOURCES += src/LongQt.cpp

