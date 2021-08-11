list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/specialmenu.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/stimulationsettings.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/gridwrapper.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/specialmenu.h"
  "${CMAKE_CURRENT_LIST_DIR}/stimulationsettings.h"
  "${CMAKE_CURRENT_LIST_DIR}/gridwrapper.h"
)

include(${CMAKE_CURRENT_LIST_DIR}/VoltageClampMenu/VoltageClampMenu.cmake)
