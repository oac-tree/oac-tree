# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

# Detecting CODAC environment
if(NOT NO_CODAC)
  # cmake warns for the existance of ``<PackageName>_ROOT`` (CODAC_ROOT in this case) variables and ignores them
  # for compatibility reasons, we set the related policy to NEW behaviour to suppress warnings and enable desired behaviour
  cmake_policy(SET CMP0074 NEW)
  find_package(CODAC)
endif()
if (CODAC_FOUND)
  # Append CODAC_CMAKE_PREFIXES to cmake seard directories, this helps cmake find packages installed in the CODAC enviorenment 
  list(APPEND CMAKE_PREFIX_PATH ${CODAC_CMAKE_PREFIXES})
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
