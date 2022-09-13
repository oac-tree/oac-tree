target_sources(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/execution_status.cpp
        ${CMAKE_CURRENT_LIST_DIR}/procedure.cpp
        ${CMAKE_CURRENT_LIST_DIR}/workspace.cpp
    )

target_include_directories(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
