add_executable(sequencer-cli)

target_sources(sequencer-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cl_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(sequencer-cli
    PRIVATE
        $ENV{CODAC_ROOT}/include
)

target_link_libraries(sequencer-cli
    PRIVATE
        sup-sequencer-shared
)

install(TARGETS sequencer-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
