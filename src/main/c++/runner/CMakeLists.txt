target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Runner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UserInterface.cpp
    )

target_include_directories(sequencer
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )
