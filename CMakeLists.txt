# Project       : SUP - Sequencer
#
# Description   : Sequencer for operational procedures
#
# Author        : Walter Van Herck (IO)
#
# Copyright (c) : 2010-2023 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.

# Set required CMake version
# This sets the default CMake behaviour through policies allowing compatibility with future versions
# Justification:
#   - CMake 3.12 - CMP0074: find_package command now searches prefixes specified by the <PackageName>_ROOT environment variable
#       CODAC systems contain conflicting environment variables which generate warnings when this policy is not set to NEW
#   - CMake 3.13 - CMP0076: relative source file paths are interpreted as being relative to the current source directory
cmake_minimum_required(VERSION 3.13)

project(sup-sequencer VERSION 3.1.0)

option(COA_COVERAGE "Generate unit test coverage information" OFF)
option(COA_PARASOFT_INTEGRATION "Parasoft integration" OFF)
option(COA_EXPORT_BUILD_TREE "Export build tree in /home/user/.cmake registry" OFF)
option(COA_BUILD_TESTS "Build unit tests" ON)
option(COA_BUILD_DOCUMENTATION "Build documentation" OFF)
option(COA_NO_CODAC "Don't look for the presence of CODAC environment" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/module)
set(CMAKE_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/config)
include(configuration)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)

include(installation)
