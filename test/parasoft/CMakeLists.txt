if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for oac-tree
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(oac-tree-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
    sup-oac-tree-shared=${CMAKE_SOURCE_DIR}/src/lib/sup/oac-tree
    oac-tree-daemon=${CMAKE_SOURCE_DIR}/src/app/oac-tree-daemon
    oac-tree-cli=${CMAKE_SOURCE_DIR}/src/app/oac-tree-cli
  TARGETS sup-oac-tree-shared oac-tree-daemon oac-tree-cli
)

get_target_property(OAC-TREE_SHARED_INCLUDE_DIRECTORIES sup-oac-tree-shared INCLUDE_DIRECTORIES)
target_include_directories(oac-tree-cpptest PUBLIC ${OAC-TREE_SHARED_INCLUDE_DIRECTORIES})

target_link_libraries(oac-tree-cpptest PUBLIC
  sup-dto::sup-dto
  sup-utils::sup-cli
  sup-utils::sup-log
  sup-utils::sup-xml
)
