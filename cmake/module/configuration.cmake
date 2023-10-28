# -----------------------------------------------------------------------------
# Modules
# -----------------------------------------------------------------------------

include(GNUInstallDirs)
include(CTest)

# -----------------------------------------------------------------------------
# CODAC enviorenment
# -----------------------------------------------------------------------------
if(NOT COA_NO_CODAC)
  find_package(CODAC OPTIONAL_COMPONENTS site-packages Python MODULE)
endif()

if(CODAC_FOUND)
  # Append CODAC_CMAKE_PREFIXES to cmake seard directories, this helps cmake find packages installed in the CODAC enviorenment
  list(APPEND CMAKE_PREFIX_PATH ${CODAC_CMAKE_PREFIXES})

  # If CODAC module provides python executable, override Python3_EXECUTABLE with it
  if(CODAC_Python_FOUND AND NOT Python3_EXECUTABLE)
    set(Python3_EXECUTABLE ${CODAC_PYTHON_EXECUTABLE})
  endif()

  # Check if operating inside a CODAC CICD system
  if(CODAC_CICD)
    message(STATUS "Detected CODAC CICD system")
    set(COA_BUILD_TESTS ON)

    # Ideally we would want a cleaner way to detect analysis builds, but are limited by the maven workflow
    if(COA_COVERAGE)
      # CODAC CICD with coverage means analysis build, enable parasoft integration
      set(COA_PARASOFT_INTEGRATION ON)
    else()
      # Regular CODAC CICD build, enable documentation for packaging
      set(COA_BUILD_DOCUMENTATION ON)
    endif()
  endif()
else()
  message(STATUS "Compiling without CODAC")
endif()

# -----------------------------------------------------------------------------
# Variables
# -----------------------------------------------------------------------------
if(COA_COVERAGE)
  # On coverage builds always build tests
  set(COA_BUILD_TESTS ON)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})

# -----------------------------------------------------------------------------
# Directories
# -----------------------------------------------------------------------------
if (NOT DEFINED TEST_OUTPUT_DIRECTORY)
  set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)
endif()

file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(sup-utils REQUIRED)
find_package(sup-dto REQUIRED)
