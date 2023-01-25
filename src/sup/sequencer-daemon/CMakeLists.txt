add_executable(sequencer-daemon)

target_sources(sequencer-daemon
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/daemon_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(sequencer-daemon PRIVATE sup-sequencer-shared)

if (SEQUENCER_CODAC)
  target_link_libraries(sequencer-daemon PRIVATE sup-log)
else()
  find_package(sup-utils REQUIRED)
  target_link_libraries(sequencer-daemon PRIVATE sup-utils::sup-log)
endif()

if(NOT SEQUENCER_CODAC)
  install(TARGETS sequencer-daemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
