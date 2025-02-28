# -----------------------------------------------------------------------------
# Main project configuration
# -----------------------------------------------------------------------------

include(GNUInstallDirs)
include(CTest)

# -----------------------------------------------------------------------------
# C++ version
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# -----------------------------------------------------------------------------
# Variables
# -----------------------------------------------------------------------------
set(LIBVERSION ${PROJECT_VERSION})
set(LIBSOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# -----------------------------------------------------------------------------
# Directories
# -----------------------------------------------------------------------------
if(NOT DEFINED TEST_OUTPUT_DIRECTORY)
  set(TEST_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_bin)
endif()

file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
