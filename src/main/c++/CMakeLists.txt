cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEFINED COVERAGE AND ${COVERAGE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_subdirectory(sup)

install(FILES
        sup/sequencer/attribute_map.h
        sup/sequencer/constants.h
        sup/sequencer/execution_status.h
        sup/sequencer/generic_utils.h
        sup/sequencer/instruction.h
        sup/sequencer/instruction_registry.h
        sup/sequencer/log.h
        sup/sequencer/named_callback_manager.h
        sup/sequencer/procedure.h
        sup/sequencer/runner.h
        sup/sequencer/sequence_parser.h
        sup/sequencer/tree_data.h
        sup/sequencer/user_interface.h
        sup/sequencer/variable.h
        sup/sequencer/variable_registry.h
        sup/sequencer/workspace.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/sequencer
)
