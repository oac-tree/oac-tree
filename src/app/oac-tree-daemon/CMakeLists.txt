target_sources(oac-tree-app-interfaces
  PRIVATE
  daemon_interface.cpp
)

add_executable(oac-tree-daemon)

target_sources(oac-tree-daemon
  PRIVATE
  main.cpp
)

target_link_libraries(oac-tree-daemon PRIVATE
  oac-tree-app-interfaces
)

install(TARGETS oac-tree-daemon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
