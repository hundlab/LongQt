list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/lqgrideditor.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/gridModel.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/conductivityeditor.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/gridDelegate.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/gridsetupwidget.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/ionchannelconfig.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/setsecondstim.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/addgridcellpvar.cpp"
)

list(APPEND headers 
  "${CMAKE_CURRENT_LIST_DIR}/lqgrideditor.h"
  "${CMAKE_CURRENT_LIST_DIR}/gridModel.h"
  "${CMAKE_CURRENT_LIST_DIR}/conductivityeditor.h"
  "${CMAKE_CURRENT_LIST_DIR}/gridDelegate.h"
  "${CMAKE_CURRENT_LIST_DIR}/gridsetupwidget.h"
  "${CMAKE_CURRENT_LIST_DIR}/ionchannelconfig.h"
  "${CMAKE_CURRENT_LIST_DIR}/setsecondstim.h"
  "${CMAKE_CURRENT_LIST_DIR}/addgridcellpvar.h"
)

list(APPEND forms
  "${CMAKE_CURRENT_LIST_DIR}/lqgrideditor.ui"
  "${CMAKE_CURRENT_LIST_DIR}/conductivityeditor.ui"
  "${CMAKE_CURRENT_LIST_DIR}/ionchannelconfig.ui"
  "${CMAKE_CURRENT_LIST_DIR}/setsecondstim.ui"
  "${CMAKE_CURRENT_LIST_DIR}/gridsetupwidget.ui"
  "${CMAKE_CURRENT_LIST_DIR}/addgridcellpvar.ui"
)

