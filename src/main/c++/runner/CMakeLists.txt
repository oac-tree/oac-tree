target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Runner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UserInterface.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Runner.h
        ${CMAKE_CURRENT_LIST_DIR}/UserInterface.h
)
