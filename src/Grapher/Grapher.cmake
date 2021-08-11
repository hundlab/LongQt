list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")


list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/graph.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphstoggle.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/qcustomplot.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/bargraph.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphs.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/linegraph.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/labelaxisticker.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/loadingprogress.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/ordinalspinbox.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/qcustomplot.h"
  "${CMAKE_CURRENT_LIST_DIR}/bargraph.h"
  "${CMAKE_CURRENT_LIST_DIR}/graph.h"
  "${CMAKE_CURRENT_LIST_DIR}/linegraph.h"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphs.h"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphstoggle.h"
  "${CMAKE_CURRENT_LIST_DIR}/labelaxisticker.h"
  "${CMAKE_CURRENT_LIST_DIR}/loadingprogress.h"
  "${CMAKE_CURRENT_LIST_DIR}/ordinalspinbox.h"
)

list(APPEND forms
  "${CMAKE_CURRENT_LIST_DIR}/linegraph.ui"
  "${CMAKE_CURRENT_LIST_DIR}/graph.ui"
  "${CMAKE_CURRENT_LIST_DIR}/bargraph.ui"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphs.ui"
  "${CMAKE_CURRENT_LIST_DIR}/choosegraphstoggle.ui"
  "${CMAKE_CURRENT_LIST_DIR}/loadingprogress.ui"
)

