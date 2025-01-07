if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sequencer
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sequencer-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
    sup-sequencer-shared=${CMAKE_SOURCE_DIR}/src/lib/sup/sequencer
    sequencer-daemon=${CMAKE_SOURCE_DIR}/src/app/sequencer-daemon
    sequencer-cli=${CMAKE_SOURCE_DIR}/src/app/sequencer-cli
  TARGETS sup-sequencer-shared sequencer-daemon sequencer-cli
)

get_target_property(SEQUENCER_SHARED_INCLUDE_DIRECTORIES sup-sequencer-shared INCLUDE_DIRECTORIES)
target_include_directories(sequencer-cpptest PUBLIC ${SEQUENCER_SHARED_INCLUDE_DIRECTORIES})

target_link_libraries(sequencer-cpptest PUBLIC
  sup-dto::sup-dto
  sup-utils::sup-cli
  sup-utils::sup-log
  sup-utils::sup-xml
)
