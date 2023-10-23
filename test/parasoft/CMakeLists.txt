if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

message(STATUS "Configuring Parasoft integration")

if(NOT PARASOFT_DIR)
  if(NOT CODAC_FOUND)
    message(FATAL_ERROR "Parasoft install location is unknown, set PARASOFT_DIR to specify one")
  endif()

  # Default to CODAC parasoft installation location in /opt
  set(PARASOFT_DIR /opt/parasoft CACHE PATH "Parasoft install location")
endif()

# Append parasoft cmake integration location to module path
list(APPEND CMAKE_MODULE_PATH "${PARASOFT_DIR}/cpptest/integration/cmake")

# Enable parasoft project generation
set(CPPTEST_PROJECT ON)

# Include Parasoft integration module
include(cpptest-project)

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
