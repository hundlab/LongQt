INCLUDEPATH += $$PWD

SOURCES +=\
    $$PWD/specialmenu.cpp \
    $$PWD/stimulationsettings.cpp \
    $$PWD/gridwrapper.cpp

HEADERS  +=\
    $$PWD/specialmenu.h \
    $$PWD/stimulationsettings.h \
    $$PWD/gridwrapper.h

include(VoltageClampMenu/VoltageClampMenu.pri)
