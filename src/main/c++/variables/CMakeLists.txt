target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/FileVariable.cpp
    )

target_include_directories(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
