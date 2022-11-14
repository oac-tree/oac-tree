# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

get_filename_component(SEQUENCER_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

# detecting CODAC environment
if (DEFINED ENV{CODAC_ROOT})
  message(STATUS "CODAC environment detected at $ENV{CODAC_ROOT}")
  set(SEQUENCER_CODAC ON)
else()
  message(STATUS "No CODAC environment detected")
  set(SEQUENCER_CODAC OFF)
endif()

# build settings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

# Directories
if (DEFINED ENV{CODAC_ROOT})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SEQUENCER_PROJECT_DIR}/target/bin)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()
