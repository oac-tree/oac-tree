# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

get_filename_component(SEQUENCER_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

# detecting CODAC environment
if (SEQUENCER_CODAC)
  if (DEFINED ENV{CODAC_ROOT})
    message(STATUS "CODAC environment detected at $ENV{CODAC_ROOT}")
  else()
    message(FATAL "No CODAC environment detected")
  endif()
else()
  message(STATUS "Compiling without CODAC")
endif()

# build settings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

# Directories
if (SEQUENCER_CODAC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SEQUENCER_PROJECT_DIR}/target/bin)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()
