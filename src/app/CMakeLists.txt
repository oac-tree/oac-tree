add_library(oac-tree-app-interfaces STATIC)

target_include_directories(oac-tree-app-interfaces PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../lib>
)

target_link_libraries(oac-tree-app-interfaces PUBLIC
  sup-oac-tree-shared
  sup-utils::sup-log
  sup-utils::sup-cli
)

add_subdirectory(oac-tree-cli)
add_subdirectory(oac-tree-daemon)
