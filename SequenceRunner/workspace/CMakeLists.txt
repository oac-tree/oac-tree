target_sources(SequenceRunner
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Instruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Procedure.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Variable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Workspace.cpp
)

target_include_directories (SequenceRunner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
