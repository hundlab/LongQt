INCLUDEPATH += $$PWD

SOURCES +=\
	$$PWD/simvarmenu.cpp\
	$$PWD/dvarmenu.cpp\
	$$PWD/mvarmenu.cpp\
	$$PWD/pvarmenu.cpp\
  $$PWD/runwidget.cpp \
  $$PWD/heartcellsim.cpp \
	$$PWD/CLISimulation.cpp \
	$$PWD/appparser.cpp \
    $$PWD/addsinglecellpvar.cpp \
  $$PWD/chooseprotowidget.cpp

HEADERS  +=\
	$$PWD/simvarmenu.h\
	$$PWD/dvarmenu.h\
	$$PWD/mvarmenu.h\
	$$PWD/pvarmenu.h \
  $$PWD/runwidget.h \
  $$PWD/heartcellsim.h \
	$$PWD/CLISimulation.h \
	$$PWD/appparser.h \
    $$PWD/addsinglecellpvar.h\
  $$PWD/chooseprotowidget.h

FORMS += \
    $$PWD/runwidget.ui \
    $$PWD/chooseprotowidget.ui\
    $$PWD/addsinglecellpvar.ui\
	$$PWD/pvarmenu.ui

