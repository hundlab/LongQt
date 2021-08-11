list(APPEND includes "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND sources
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampdelegate.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampmodel.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampsetupwidget.cpp"
)

list(APPEND headers
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampdelegate.h"
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampmodel.h"
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampsetupwidget.h"
)

list(APPEND forms
  "${CMAKE_CURRENT_LIST_DIR}/voltageclampsetupwidget.ui"
)
