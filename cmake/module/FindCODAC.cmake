# CMake helper for compilation in CODAC systems
#
# This module provides:
#   CODAC_DIR: root directory of CODAC Enviorenment
#   CODAC_VERSION: version of CODAC Enviorenment
#   CODAC_CMAKE_PREFIXES: list of prefixes to help cmake find CODAC packages
#   CODAC_CICD: operating in a CODAC CICD system if true
#   CODAC_FOUND: required variable for find_package, helps check if we are in a CODAC Enviorenment
# 
# Additional optional components:
#
#   site-packages:
#     CODAC_PYTHONXY_SITE_PACKAGES: python vX.Y CODAC site-packages directory
#     CODAC_site-packages_FOUND: TRUE if any site-packages dirs were found
#     
#   Python:
#     CODAC_PYTHON_EXECUTABLE: CODAC system python executable, 
#       when paired with site-packages this executable is aliased with the corresponding site-packages
#     CODAC_Python_VERSION_MAJOR: CODAC system python major version
#     CODAC_Python_VERSION_MINOR: CODAC system python minor version
#     CODAC_Python_VERSION_PATCH: CODAC system python patch version
#     CODAC_Python_VERSION: CODAC system python major.minor version
#     CODAC_Python_FOUND: TRUE if CODAC system python was found

include(FindPackageHandleStandardArgs)

# --- Find Python site-packages ---
macro(_CODAC_find_site_packages)
  file(GLOB _site_packages_dirs LIST_DIRECTORIES true ${CODAC_DIR}/**/python*/site-packages)
  foreach(_site_packages_dir ${_site_packages_dirs})
    if(_site_packages_dir MATCHES ".*python([0-9]+).([0-9]+).*$")
      set(CODAC_PYTHON${CMAKE_MATCH_1}${CMAKE_MATCH_2}_SITE_PACKAGES ${_site_packages_dir})
      message(STATUS "Found CODAC python site-packages CODAC_PYTHON${CMAKE_MATCH_1}${CMAKE_MATCH_2}_SITE_PACKAGES: ${CODAC_PYTHON${CMAKE_MATCH_1}${CMAKE_MATCH_2}_SITE_PACKAGES}")
      set(CODAC_site-packages_FOUND TRUE)
    endif()
  endforeach()
endmacro()

# --- Find Python component ---
macro(_CODAC_find_python)
  find_program(CODAC_PYTHON_EXECUTABLE NAMES python3)
  if(NOT CODAC_PYTHON_EXECUTABLE)
    message(STATUS "CODAC Python executable not found")
    unset(CODAC_PYTHON_EXECUTABLE)
  endif()

  # Resolve executable symlinks
  # CODAC systems uses alternatives to select the desired default binary
  # we resolve symlinks to make sure we have a unchanging configuration after running cmake
  if(CODAC_PYTHON_EXECUTABLE)
    execute_process(
      COMMAND readlink -f ${CODAC_PYTHON_EXECUTABLE}
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET)
    if(_result EQUAL 0)
      set(CODAC_PYTHON_EXECUTABLE ${_output})
    endif()

    # Find CODAC Python version
    execute_process(
      COMMAND ${CODAC_PYTHON_EXECUTABLE} --version
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET)
    if(_result EQUAL 0)
      if(_output MATCHES " v?([0-9]+)\\.([0-9]+)\\.([0-9]+)$")
        set(CODAC_Python_VERSION_MAJOR "${CMAKE_MATCH_1}")
        set(CODAC_Python_VERSION_MINOR "${CMAKE_MATCH_2}")
        set(CODAC_Python_VERSION_PATCH "${CMAKE_MATCH_3}")
        set(CODAC_Python_VERSION ${CODAC_Python_VERSION_MAJOR}.${CODAC_Python_VERSION_MINOR})
        set(CODAC_Python_FOUND TRUE)
      else()
        set(CODAC_Python_FOUND FALSE)
      endif()
    endif()

    # Alias python with a PYTHONPATH to CODAC site-packages if available
    if(CODAC_PYTHON${CODAC_Python_VERSION_MAJOR}${CODAC_Python_VERSION_MINOR}_SITE_PACKAGES)
      message(STATUS "CODAC_PYTHON_EXECUTABLE using site-packages: CODAC_PYTHON${CODAC_Python_VERSION_MAJOR}${CODAC_Python_VERSION_MINOR}_SITE_PACKAGES")

      # Create bash file to alias Python executable with site-packages
      # we do this because cmake doesn't like non monlithic executable names in package_EXECUTABLE macros
      set(_file ${CMAKE_CURRENT_BINARY_DIR}/CODAC_PYTHON_EXECUTABLE.sh)
      file(WRITE ${_file}
"#!/bin/bash
export PYTHONPATH=${CODAC_PYTHON${CODAC_Python_VERSION_MAJOR}${CODAC_Python_VERSION_MINOR}_SITE_PACKAGES}
${CODAC_PYTHON_EXECUTABLE} \"$@\"")
      configure_file(${_file} ${_file} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
    
      set(CODAC_PYTHON_EXECUTABLE ${_file})
  endif()

  else()
    set(CODAC_Python_FOUND FALSE)
  endif()
endmacro()

# --- Find CODAC Enviorenment ---
if(DEFINED ENV{CODAC_ROOT})
    set(CODAC_DIR $ENV{CODAC_ROOT})

    if(DEFINED ENV{CODAC_VERSION})
      set(CODAC_VERSION $ENV{CODAC_VERSION})
    elseif(${CODAC_DIR} MATCHES ".*codac-([0-9]+\\.[0-9]+)$")
      # This is unlikely to be needed in full CODAC systems
      # convenient for local systesms with CODAC packages
      set(CODAC_VERSION ${CMAKE_MATCH_1})
    endif()

    # Prefixes to help cmake find config files for CODAC packages
    set(CODAC_CMAKE_PREFIXES ${CODAC_DIR} ${CODAC_DIR}/common)

    set(_codac_ci_env $ENV{CI})
    if(_codac_ci_env STREQUAL "true")
      # Inside CODAC CICD system
      set(CODAC_CICD TRUE)
    else()
      set(CODAC_CICD FALSE)
    endif()

    set(CODAC_FOUND TRUE)
else()
    set(CODAC_FOUND FALSE)
endif()


# --- Find CODAC components ---
if(CODAC_FOUND)
    if("site-packages" IN_LIST CODAC_FIND_COMPONENTS)
        _CODAC_find_site_packages()
        list(REMOVE_ITEM CODAC_FIND_COMPONENTS "site-packages")
    endif()
    if("Python" IN_LIST CODAC_FIND_COMPONENTS)
        _CODAC_find_python()
        list(REMOVE_ITEM CODAC_FIND_COMPONENTS "Python")
    endif()
endif()

find_package_handle_standard_args(
  CODAC
  VERSION_VAR CODAC_VERSION
  REQUIRED_VARS CODAC_DIR CODAC_VERSION CODAC_CMAKE_PREFIXES
  HANDLE_COMPONENTS)
