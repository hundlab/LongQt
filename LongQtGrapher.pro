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
INCLUDEPATH += ./modellib

linux {
    debug {
        QMAKE_CXXFLAGS += -g
        QMAKE_CXXFLAGS_RELEASE -= -O2
    }
    relase {
        QMAKE_CXXFLAGS += -static
    }
    TARGET = LongQtGrapher.out

    DESTDIR = ./build 
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/obj
    RCC_DIR = ./build/obj
    UI_DIR = ./build/obj
}

SOURCES += LongQtGrapher.cpp\
        dialog.cpp \
    modellib/grid.cpp \
    modellib/fiber.cpp \
    modellib/cell_kernel.cpp \
    modellib/cell.cpp \
    modellib/kurata08.cpp \
    modellib/gpbatrial.cpp \
    modellib/gridCell.cpp \
    modellib/protocol.cpp \
    modellib/voltageClampProtocol.cpp \
    modellib/currentClampProtocol.cpp \
    modellib/gridProtocol.cpp \
    modellib/measure_kernel.cpp \
    modellib/measure.cpp \
    modellib/iobase.cpp \
    gridSettup.cpp \
    modellib/hrd09.cpp \
    modellib/hrd09_bz.cpp \
    modellib/tnnp04.cpp \
    chooseProtoWidget.cpp \
    runWidget.cpp \
    varmenu.cpp \
    heart_cell_sim.cpp \
    qcustomplot.cpp \
    modellib/gpbatrialRyr.cpp \
    linegraph.cpp \
    bargraph.cpp \
    choosegraphs.cpp \
    choosegraphstoggle.cpp

HEADERS  +=    dialog.h \
    modellib/kurata08.h \
    modellib/tridag.h \
    modellib/gridCell.h \
    modellib/gpbatrial.h \
    modellib/grid.h \
    modellib/fiber.h \
    modellib/node.h \
    modellib/cell_kernel.h \
    modellib/cell.h \
    modellib/protocol.h \
    modellib/voltageClampProtocol.h \
    modellib/currentClampProtocol.h \
    modellib/gridProtocol.h \
    modellib/measure_kernel.h \
    modellib/measure.h \
    modellib/iobase.h \
    gridSettup.h \
    modellib/hrd09.h \
    modellib/tnnp04.h \
    chooseProtoWidget.h \
    runWidget.h \
    varmenu.h \
    heart_cell_sim.h \
    qcustomplot.h \
    modellib/cellUtils.h \
    guiUtils.h \
    linegraph.h \
    bargraph.h \
    choosegraphs.h \
    choosegraphstoggle.h

FORMS    += dialog.ui \
    linegraph.ui \
    bargraph.ui \
    choosegraphs.ui \
    choosegraphstoggle.ui
