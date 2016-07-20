#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LongQt
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

CONFIG += c++11
INCLUDEPATH += ./modellib ./modellib/cell ./modellib/structure ./modellib/protocol ./gui ./gui/simulaiton ./gui/graph

linux {
    debug {
        QMAKE_CXXFLAGS += -g
        QMAKE_CXXFLAGS_RELEASE -= -O2
    }
    relase {
        QMAKE_CXXFLAGS += -static
    }
    TARGET = LongQt.out

    DESTDIR = ./build 
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/obj
    RCC_DIR = ./build/obj
    UI_DIR = ./build/obj
}

SOURCES += LongQt.cpp \
    gui/simulaiton/*.cpp \
    gui/graph/*.cpp \
    modellib/*.cpp \
    modellib/cell/*.cpp \
    modellib/structure/*.cpp \
    modellib/protocol/*.cpp

HEADERS  += gui/*.h \
    gui/simulaiton/*.h \
    gui/graph/*.h \
    modellib/*.h \
    modellib/cell/*.h \
    modellib/structure/*.h \
    modellib/protocol/*.h

FORMS    += gui/graph/*.ui \

