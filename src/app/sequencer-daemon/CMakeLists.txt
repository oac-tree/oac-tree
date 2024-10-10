add_executable(sequencer-daemon)

target_sources(sequencer-daemon
  PRIVATE
  daemon_interface.cpp
  main.cpp
)

target_link_libraries(sequencer-daemon PRIVATE
  sup-sequencer-shared
  sup-utils::sup-log
  sup-utils::sup-cli
)

install(TARGETS sequencer-daemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
