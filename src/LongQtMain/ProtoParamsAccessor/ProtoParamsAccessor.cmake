list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/simvar.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvbool.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvcell.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvcellopts.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvdir.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvdouble.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvfile.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/simvint.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/simvar.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvbool.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvcell.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvcellopts.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvdir.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvdouble.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvfile.h"
  "${CMAKE_CURRENT_LIST_DIR}/simvint.h"
)
