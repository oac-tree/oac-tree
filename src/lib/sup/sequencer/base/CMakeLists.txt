target_sources(sup-sequencer-shared
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/attribute_map.cpp
    ${CMAKE_CURRENT_LIST_DIR}/exceptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/generic_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/log_severity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/scope_guard.cpp
)
