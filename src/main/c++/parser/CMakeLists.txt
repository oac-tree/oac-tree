target_sources(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/InstructionParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ProcedureParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParserImpl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/VariableParser.cpp
)

target_include_directories(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
