#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MyConcurrentModel.out
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
INCLUDEPATH += ./modellib

SOURCES += main.cpp\
        dialog.cpp \
    modellib/cell_kernel.cpp \
    modellib/cell.cpp \
    modellib/kurata08.cpp \
    modellib/protocol.cpp \
    modellib/measure_kernel.cpp \
    modellib/measure.cpp \
    modellib/iobase.cpp \
    varmenu.cpp \
    heart_cell_sim.cpp \
    myoutput.cpp    \
    qcustomplot.cpp

HEADERS  +=    dialog.h \
    modellib/kurata08.h \
    modellib/cell_kernel.h \
    modellib/cell.h \
    modellib/protocol.h \
    modellib/measure_kernel.h \
    modellib/measure.h \
    modellib/iobase.h \
    varmenu.h \
    heart_cell_sim.h \
    myoutput.h  \
    qcustomplot.h

FORMS    += dialog.ui

