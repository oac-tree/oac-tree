target_sources(sequencer-daemon
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/daemon_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    )

target_include_directories(sequencer-daemon
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
