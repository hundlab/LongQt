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
INCLUDEPATH += ./modellib ./modellib/cell ./modellib/structure ./modellib/protocol ./gui ./gui/simulaiton ./gui/graph ./lib
RESOURCES = LongQt.qrc

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

SOURCES += LongQt.cpp \
./gui/simulaiton/chooseProtoWidget.cpp\
./gui/simulaiton/runWidget.cpp\
./gui/simulaiton/varmenu.cpp\
./gui/simulaiton/gridSettup.cpp\
./gui/simulaiton/heart_cell_sim.cpp\
./gui/graph/graph.cpp\
./gui/graph/choosegraphstoggle.cpp\
./gui/graph/qcustomplot.cpp\
./gui/graph/bargraph.cpp\
./gui/graph/choosegraphs.cpp\
./gui/graph/linegraph.cpp\
./modellib/measure.cpp\
./modellib/measure_kernel.cpp\
./modellib/cell/gpbatrialRyr.cpp\
./modellib/cell/hrd09_bz.cpp\
./modellib/cell/cell.cpp\
./modellib/cell/cell_kernel.cpp\
./modellib/cell/tnnp04.cpp\
./modellib/cell/kurata08.cpp\
./modellib/cell/hrd09.cpp\
./modellib/cell/gpbatrial.cpp\
./modellib/cell/gridCell.cpp\
./modellib/iobase.cpp\
./modellib/structure/fiber.cpp\
./modellib/structure/grid.cpp\
./modellib/protocol/gridProtocol.cpp\
./modellib/protocol/protocol.cpp\
./modellib/protocol/currentClampProtocol.cpp\
./modellib/protocol/voltageClampProtocol.cpp\
./lib/argHandler.cpp

HEADERS  += ./gui/simulaiton/heart_cell_sim.h\
./gui/simulaiton/chooseProtoWidget.h\
./gui/simulaiton/gridSettup.h\
./gui/simulaiton/varmenu.h\
./gui/simulaiton/runWidget.h\
./gui/graph/qcustomplot.h\
./gui/graph/bargraph.h\
./gui/graph/graph.h\
./gui/graph/linegraph.h\
./gui/graph/choosegraphs.h\
./gui/graph/choosegraphstoggle.h\
./gui/guiUtils.h\
./modellib/measure_kernel.h\
./modellib/measure.h\
./modellib/tridag.h\
./modellib/cell/gpbatrial.h\
./modellib/cell/gridCell.h\
./modellib/cell/hrd09.h\
./modellib/cell/cell.h\
./modellib/cell/kurata08.h\
./modellib/cell/tnnp04.h\
./modellib/cell/cell_kernel.h\
./modellib/cell/gpbatrialRyr.h\
./modellib/iobase.h\
./modellib/cellUtils.h\
./modellib/structure/grid.h\
./modellib/structure/node.h\
./modellib/structure/fiber.h\
./modellib/protocol/voltageClampProtocol.h\
./modellib/protocol/currentClampProtocol.h\
./modellib/protocol/protocol.h\
./modellib/protocol/gridProtocol.h\
./lib/argHandler.h

FORMS    += ./gui/graph/linegraph.ui \
./gui/graph/graph.ui \
./gui/graph/bargraph.ui \
./gui/graph/choosegraphs.ui \
./gui/graph/choosegraphstoggle.ui


