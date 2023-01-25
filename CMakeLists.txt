cmake_minimum_required(VERSION 3.5)

project(SUP-SEQUENCER VERSION 2.0.0)

option(SEQUENCER_EXPORT_BUILDTREE "Export build tree in /home/user/.cmake registry" OFF)
option(SEQUENCER_CODAC "Compilation for CODAC environment" OFF)
option(COVERAGE "Generate unit test coverage information" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/cmake)
include(configuration)

add_subdirectory(src)
add_subdirectory(test)

include(installation)
