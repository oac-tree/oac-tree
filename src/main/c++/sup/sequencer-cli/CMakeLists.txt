add_executable(sequencer-cli)

target_sources(sequencer-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cl_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(sequencer-cli PRIVATE sup-sequencer-shared)

if (SEQUENCER_CODAC)
  target_link_libraries(sequencer-cli PRIVATE sup-log)
else()
  find_package(sup-utils REQUIRED)
  target_link_libraries(sequencer-cli PRIVATE sup-utils::sup-log)
endif()

if(NOT SEQUENCER_CODAC)
  install(TARGETS sequencer-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
