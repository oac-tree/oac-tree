target_sources(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AttributeMap.cpp
)

target_include_directories(sequencer
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
