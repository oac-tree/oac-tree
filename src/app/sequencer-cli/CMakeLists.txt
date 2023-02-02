add_executable(sequencer-cli)

target_compile_features(sequencer-cli PRIVATE cxx_std_11)

target_sources(sequencer-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cl_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(sequencer-cli PRIVATE sup-sequencer-shared)

find_package(sup-utils REQUIRED)
target_link_libraries(sequencer-cli PRIVATE sup-utils::sup-log)

install(TARGETS sequencer-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
