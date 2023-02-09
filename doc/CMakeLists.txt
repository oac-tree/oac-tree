# Setup target for documentation generation.

# --- Finding Doxygen ---
find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message(INFO "No doxygen found, skipping documentation generation")
  return()
endif()

# --- Finding Sphinx ---
find_package(Sphinx)
if(NOT Sphinx_FOUND)
  message(INFO "No Sphinx found, skipping documentation generation")
  return()
endif()

# Just output XML since this is what Breathe consumes.
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_XML YES)

# Reduce Doxygen verbosity. We only care about warnings and errors and we also
# don't care about undocumented symbols.
set(DOXYGEN_QUIET YES)
set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)

# Sphinx output directory 
set(SPHINX_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/sphinx_build)

doxygen_add_docs(doxygen-doc ${PROJECT_SOURCE_DIR}/src COMMENT
                 "Generating API documentation with Doxygen")

configure_file(${CMAKE_CONFIG_PATH}/sphinx-conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py @ONLY)

add_custom_target(sphinx-doc ALL
  COMMAND ${SPHINX_BUILD_EXECUTABLE} -b html -c ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR} ${SPHINX_BUILD_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating API documentation with Sphinx"
  VERBATIM)

add_dependencies(sphinx-doc doxygen-doc)

install(DIRECTORY ${SPHINX_BUILD_DIR}/ DESTINATION ${CMAKE_INSTALL_DOCDIR}
  PATTERN .doctrees EXCLUDE
  PATTERN .buildinfo EXCLUDE)
