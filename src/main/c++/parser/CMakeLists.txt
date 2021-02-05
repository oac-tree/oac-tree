target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/InstructionParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ProcedureParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParserImpl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/VariableParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TreeData.cpp
)

target_include_directories(sequencer
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )
