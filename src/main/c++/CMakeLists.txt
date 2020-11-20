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

find_package(LibXml2 REQUIRED)

add_subdirectory(parser)
add_subdirectory(runner)
add_subdirectory(procedure)
add_subdirectory(instructions)

target_include_directories(sequencer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${LIBXML2_INCLUDE_DIR}
    PRIVATE $ENV{CODAC_ROOT}/include
    )
target_link_libraries(sequencer ${LIBXML2_LIBRARIES})
