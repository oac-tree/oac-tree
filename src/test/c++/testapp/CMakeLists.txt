add_executable(testapp)

target_sources(testapp
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_procedure_factory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_ui.cpp
)

target_link_libraries(testapp PRIVATE sup-sequencer-shared)
