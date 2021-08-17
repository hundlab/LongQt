list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/addsinglecellpvar.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/pvarmenu.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/addsinglecellpvar.h"
  "${CMAKE_CURRENT_LIST_DIR}/pvarmenu.h"
) 

list(APPEND forms 
  "${CMAKE_CURRENT_LIST_DIR}/addsinglecellpvar.ui"
  "${CMAKE_CURRENT_LIST_DIR}/pvarmenu.ui"
)
