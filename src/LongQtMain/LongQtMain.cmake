list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/CLISimulation.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/appparser.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/chooseprotowidget.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/longqtmainwindow.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/oneitemlayout.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/protochooser.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/runwidget.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simfiles.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvarmenu.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/checkandwarn.cpp"
)

list(APPEND headers 
  "${CMAKE_CURRENT_LIST_DIR}/CLISimulation.h"
  "${CMAKE_CURRENT_LIST_DIR}/appparser.h"
  "${CMAKE_CURRENT_LIST_DIR}/chooseprotowidget.h"
  "${CMAKE_CURRENT_LIST_DIR}/longqtmainwindow.h"
  "${CMAKE_CURRENT_LIST_DIR}/oneitemlayout.h"
  "${CMAKE_CURRENT_LIST_DIR}/protochooser.h"
  "${CMAKE_CURRENT_LIST_DIR}/runwidget.h"
  "${CMAKE_CURRENT_LIST_DIR}/simfiles.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvarmenu.h"
  "${CMAKE_CURRENT_LIST_DIR}/checkandwarn.h"
)

list(APPEND forms 
  "${CMAKE_CURRENT_LIST_DIR}/chooseprotowidget.ui"
  "${CMAKE_CURRENT_LIST_DIR}/longqtmainwindow.ui" 
  "${CMAKE_CURRENT_LIST_DIR}/runwidget.ui"
  "${CMAKE_CURRENT_LIST_DIR}/simvarmenu.ui"
  "${CMAKE_CURRENT_LIST_DIR}/checkandwarn.ui"
)

include(${CMAKE_CURRENT_LIST_DIR}/CellParamsMenu/CellParamsMenu.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/DataOutputMenu/DataOutputMenu.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ProtoDependantMenu/ProtoDependantMenu.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ProtoParamsAccessor/ProtoParamsAccessor.cmake)
