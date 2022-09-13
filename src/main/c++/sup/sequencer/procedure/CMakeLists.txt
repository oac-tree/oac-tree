target_sources(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ExecutionStatus.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Procedure.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Workspace.cpp
    )

target_include_directories(sup-sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
