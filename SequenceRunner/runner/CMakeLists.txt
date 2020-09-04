target_sources(SequenceRunner
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Runner.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Runner.h
        ${CMAKE_CURRENT_LIST_DIR}/UserInterface.h
)
