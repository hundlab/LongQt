#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent

TARGET = MyConcurrentModel.out
TEMPLATE = app

#QMAKE_MAC_SDK = macosx10.11

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
INCLUDEPATH += ./modellib

SOURCES += main.cpp\
        dialog.cpp \
    modellib/cell.cpp \
    modellib/kurata08.cpp \
#    modellib/kurata08mod.cpp \
    modellib/proto.cpp \
    varmenu.cpp \
    heart_cell_sim.cpp \
    myoutput.cpp    \
    qcustomplot.cpp

HEADERS  +=    dialog.h \
    modellib/kurata08.h \
#    modellib/kurata08mod.h \
    modellib/cell.h \
    modellib/proto.h \
    varmenu.h \
    heart_cell_sim.h \
    myoutput.h  \
    qcustomplot.h

FORMS    += dialog.ui

