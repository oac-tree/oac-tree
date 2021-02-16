target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ExecutionStatus.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Procedure.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Workspace.cpp
    )

target_include_directories(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
