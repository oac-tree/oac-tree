cmake_minimum_required(VERSION 3.5)

project(sup-sequencer VERSION 2.0.0)

option(COVERAGE "Generate unit test coverage information" OFF)
option(SEQUENCER_EXPORT_BUILDTREE "Export build tree in /home/user/.cmake registry" OFF)
option(BUILD_TESTS "Build unit tests" ON)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(NO_CODAC "Don't look for the presence of CODAC environment" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/module)
set(CMAKE_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/config)
include(configuration)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_subdirectory(src)
if (BUILD_TESTS)
  add_subdirectory(test)
endif()
if(COA_BUILD_DOCUMENTATION)
  add_subdirectory(doc)
endif()

include(installation)
