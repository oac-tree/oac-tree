cmake_minimum_required(VERSION 3.5)

project(Sequencer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(sequencer)

set_target_properties(sequencer PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
)

add_executable(sequencer-cli)

find_package(LibXml2 REQUIRED)

add_subdirectory(include)
add_subdirectory(base)
add_subdirectory(parser)
add_subdirectory(runner)
add_subdirectory(procedure)
add_subdirectory(instructions)
add_subdirectory(variables)
add_subdirectory(epics)
add_subdirectory(cli)

target_include_directories(sequencer
    PRIVATE ${LIBXML2_INCLUDE_DIR}
    PRIVATE $ENV{CODAC_ROOT}/include
    )

target_link_directories(sequencer
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer
    PRIVATE
        ${LIBXML2_LIBRARIES}
    PUBLIC
        ccs-common
        ccs-types
        ccs-base
    )

target_include_directories(sequencer-cli
    PRIVATE $ENV{CODAC_ROOT}/include
    )

target_link_directories(sequencer-cli
    PRIVATE $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer-cli
    PRIVATE
        sequencer
        seq-epics
    )
