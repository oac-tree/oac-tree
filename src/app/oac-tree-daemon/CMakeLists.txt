add_executable(oac-tree-daemon)

target_sources(oac-tree-daemon
  PRIVATE
  daemon_interface.cpp
  main.cpp
)

target_link_libraries(oac-tree-daemon PRIVATE
  sup-oac-tree-shared
  sup-utils::sup-log
  sup-utils::sup-cli
)

install(TARGETS oac-tree-daemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
