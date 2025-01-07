# Setup CODAC environment related to continuous integration, and documentation.

include(FindPackageMessage)

if(NOT COA_NO_CODAC)
  find_package(CODAC OPTIONAL_COMPONENTS site-packages Python MODULE QUIET)
endif()

if(CODAC_FOUND)
  set(CODAC_FOUND_MESSAGE "Building with CODAC")

  # Append CODAC_CMAKE_PREFIXES to cmake seard directories, this helps cmake find packages installed in the CODAC enviorenment
  list(APPEND CMAKE_PREFIX_PATH ${CODAC_CMAKE_PREFIXES})

  # If CODAC module provides python executable, override Python3_EXECUTABLE with it
  if(CODAC_Python_FOUND AND NOT Python3_EXECUTABLE)
    set(Python3_EXECUTABLE ${CODAC_PYTHON_EXECUTABLE})
  endif()

  # Check if operating inside a CODAC CICD system
  if(CODAC_CICD)
    string(APPEND CODAC_FOUND_MESSAGE " CICD environment")

    set(COA_BUILD_TESTS ON)

    # Ideally we would want a cleaner way to detect analysis builds, but are limited by the maven workflow
    if(COA_COVERAGE)
      # CODAC CICD with coverage means analysis build, enable parasoft integration
      set(COA_PARASOFT_INTEGRATION ON)
    else()
      # Regular CODAC CICD build, enable documentation for packaging
      set(COA_BUILD_DOCUMENTATION ON)
    endif()
  else()
    string(APPEND CODAC_FOUND_MESSAGE " environment")
  endif()

  if(CODAC_DOCS)
    string(APPEND CODAC_FOUND_MESSAGE " and will build documentation")

    set(COA_BUILD_DOCUMENTATION ON)
  endif()

  find_package_message(
    CODAC_DETAILS
    "${CODAC_FOUND_MESSAGE}: ${CODAC_DIR} (version \"${CODAC_VERSION}\")"
    "[${CODAC_FOUND}][${CODAC_DIR}][${CODAC_CICD}][v${CODAC_VERSION}]"
  )
else()
  find_package_message(CODAC_DETAILS "Building without CODAC environment" "[${CODAC_FOUND}]")
endif()

if(COA_COVERAGE)
  # On coverage builds always build tests
  set(COA_BUILD_TESTS ON)
endif()
