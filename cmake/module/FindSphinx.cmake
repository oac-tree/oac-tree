# CMake helper to look for Sphinx
#
# This module looks for Sphinx through Python modules only, this is intentional
#
# This module provides:
#   SPHINX_BUILD_EXECUTABLE:
#   SPHINX_PYTHON_EXECUTABLE:
#   Sphinx_build_FOUND: TRUE if sphinx-build was found
# 
# Additional optional components:
#   components in this module are treated as sphinx extensions, and are used to check for their presence
#   e.g:
#     find_package(Sphinx COMPONENTS breathe)
# 
#   Sphinx_component_FOUND: TRUE if provided sphinx extension was found

include(FindPackageHandleStandardArgs)

# --- Find python module macro ---
macro(_Sphinx_find_module _name _module)
  string(TOUPPER "${_name}" _name_upper)
  if(SPHINX_PYTHON_EXECUTABLE)
    execute_process(
      COMMAND ${SPHINX_PYTHON_EXECUTABLE} -m${_module} --version
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _output
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET)
    if(_result EQUAL 0)
      if(_output MATCHES " v?([0-9]+\\.[0-9]+\\.[0-9]+)$")
        set(SPHINX_${_name_upper}_VERSION "${CMAKE_MATCH_1}")
      endif()

      set(SPHINX_${_name_upper}_EXECUTABLE ${SPHINX_PYTHON_EXECUTABLE} -m${_module})
      set(Sphinx_${_name}_FOUND TRUE)
    else()
      set(Sphinx_${_name}_FOUND FALSE)
    endif()
  else()
    set(Sphinx_${_name}_FOUND FALSE)
  endif()
  unset(_name_upper)
endmacro()

# --- Find sphinx extension macro ---
macro(_Sphinx_find_extension _extension)
  if(SPHINX_PYTHON_EXECUTABLE)
    execute_process(
      COMMAND ${SPHINX_PYTHON_EXECUTABLE} -c "import ${_extension}"
      RESULT_VARIABLE _result)
    if(_result EQUAL 0)
      set(Sphinx_${_extension}_FOUND TRUE)
    else()
      set(Sphinx_${_extension}_FOUND FALSE)
    endif()
  endif()
endmacro()


# --- Find Sphinx-build & Sphinx-quickstart ---
if(NOT SPHINX_PYTHON_EXECUTABLE)
  if(NOT Python3_EXECUTABLE)
    find_package(Python3 QUIET)
  endif()
  if(Python3_EXECUTABLE)
      set(SPHINX_PYTHON_EXECUTABLE ${Python3_EXECUTABLE})
  endif()
endif()

# --- Find Sphinx-build & Sphinx-quickstart ---
if(SPHINX_PYTHON_EXECUTABLE)
    _Sphinx_find_module(build sphinx.cmd.build)
endif()

# --- Find Sphinx-build & Sphinx-quickstart ---
if(Sphinx_build_FOUND)
  foreach(_Sphinx_component IN LISTS Sphinx_FIND_COMPONENTS)
    if(_Sphinx_component STREQUAL "build")
      # Do nothing, sphinx-build is always required and is implicitly found.
      continue()
    endif()
    _Sphinx_find_extension(${_Sphinx_component})
  endforeach()
  unset(_Sphinx_component)
endif()

find_package_handle_standard_args(
  Sphinx
  VERSION_VAR SPHINX_BUILD_VERSION
  REQUIRED_VARS SPHINX_BUILD_EXECUTABLE SPHINX_BUILD_VERSION
  HANDLE_COMPONENTS)
