target_sources(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Variable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/VariableRegistry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LocalVariable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileVariable.cpp
    )

target_include_directories(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
