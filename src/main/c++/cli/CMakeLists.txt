target_sources(sequencer-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/CLInterface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    )

target_include_directories(sequencer-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
