target_sources(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AttributeMap.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TreeData.cpp
)

target_include_directories(sequencer-obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    )
