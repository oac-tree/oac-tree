target_sources(oac-tree-app-interfaces
  PRIVATE
  cl_interface.cpp
)

add_executable(oac-tree-cli)

target_sources(oac-tree-cli
  PRIVATE
  main.cpp
)

target_link_libraries(oac-tree-cli PRIVATE
  oac-tree-app-interfaces
)

install(TARGETS oac-tree-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
