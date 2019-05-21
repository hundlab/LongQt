INCLUDEPATH += $$PWD

SOURCES +=\
    $$PWD/CLISimulation.cpp \
    $$PWD/appparser.cpp \
    $$PWD/chooseprotowidget.cpp \
    $$PWD/longqtmainwindow.cpp \
    $$PWD/oneitemlayout.cpp \
    $$PWD/protochooser.cpp \
    $$PWD/runwidget.cpp \
    $$PWD/simfiles.cpp \
    $$PWD/simvarmenu.cpp

HEADERS  +=\
    $$PWD/CLISimulation.h \
    $$PWD/appparser.h \
    $$PWD/chooseprotowidget.h \
    $$PWD/longqtmainwindow.h \
    $$PWD/oneitemlayout.h \
    $$PWD/protochooser.h \
    $$PWD/runwidget.h \
    $$PWD/simfiles.h \
    $$PWD/simvarmenu.h \

FORMS += \
    $$PWD/chooseprotowidget.ui\
    $$PWD/longqtmainwindow.ui \ 
    $$PWD/runwidget.ui \
    $$PWD/simvarmenu.ui

include(CellParamsMenu/CellParamsMenu.pri)
include(DataOutputMenu/DataOutputMenu.pri)
include(ProtoDependantMenu/ProtoDependantMenu.pri)
include(ProtoParamsAccessor/ProtoParamsAccessor.pri)
