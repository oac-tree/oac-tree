add_executable(oac-tree-cli)

target_sources(oac-tree-cli
  PRIVATE
  cl_interface.cpp
  main.cpp
)

target_link_libraries(oac-tree-cli PRIVATE
  sup-oac-tree-shared
  sup-utils::sup-log
  sup-utils::sup-cli
)

install(TARGETS oac-tree-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
