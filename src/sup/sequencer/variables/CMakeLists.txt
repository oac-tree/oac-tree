target_sources(sup-sequencer-shared
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/variable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/variable_registry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/local_variable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/file_variable.cpp
)
