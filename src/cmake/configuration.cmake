# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

# Detecting CODAC environment
if (NOT NO_CODAC AND DEFINED ENV{CODAC_ROOT})
    message(STATUS "CODAC environment detected at $ENV{CODAC_ROOT}")
    list(APPEND CMAKE_PREFIX_PATH $ENV{CODAC_ROOT} $ENV{CODAC_ROOT}/common)
else()
  message(STATUS "Compiling without CODAC")
endif()

# Build settings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

# Directories
if (NOT DEFINED TEST_OUTPUT_DIRECTORY)
  set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)
endif()

file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
