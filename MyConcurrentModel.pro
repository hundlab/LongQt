#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyConcurrentModel
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
INCLUDEPATH += ./modellib

SOURCES += main.cpp\
        dialog.cpp \
    modellib/cell.cpp \
    modellib/kurata08.cpp \
#    modellib/kurata08mod.cpp \
    modellib/proto.cpp \
    myoutput.cpp

HEADERS  += dialog.h \
    modellib/cell.h \
    modellib/kurata08.h \
#    modellib/kurata08mod.h \
    modellib/proto.h \
    myoutput.h

FORMS    += dialog.ui
