target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AsyncWrapper.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CompoundInstruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Condition.cpp
        ${CMAKE_CURRENT_LIST_DIR}/DecoratorInstruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Instruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/InstructionRegistry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Inverter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ParallelSequence.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Repeat.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Sequence.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Wait.cpp
    )

target_include_directories(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )

add_subdirectory(ThirdParty)