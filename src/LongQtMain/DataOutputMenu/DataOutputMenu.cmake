list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputitem.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputselectionmodel.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputselectionwidget.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputitem.h"
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputselectionmodel.h"
  "${CMAKE_CURRENT_LIST_DIR}/dataoutputselectionwidget.h"
)

list(APPEND forms
  "${CMAKE_CURRENT_LIST_DIR}/mvarmenu.ui"
)
