target_sources(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Variable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/VariableRegistry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LocalVariable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileVariable.cpp
    )

target_include_directories(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
