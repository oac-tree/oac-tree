cmake_minimum_required(VERSION 3.5)

project(Sequencer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEFINED COVERAGE AND ${COVERAGE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(sequencer-obj OBJECT)
set_property(TARGET sequencer-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_executable(sequencer-cli)

add_executable(sequencer-daemon)

add_library(sequencer-shared SHARED)
add_library(sequencer-static STATIC)

find_package(LibXml2 REQUIRED)

add_subdirectory(include)
add_subdirectory(base)
add_subdirectory(parser)
add_subdirectory(runner)
add_subdirectory(procedure)
add_subdirectory(instructions)
add_subdirectory(variables)
add_subdirectory(cli)
add_subdirectory(daemon)

if(NOT DEFINED ENV{NO_PLUGINS})
    add_subdirectory(plugins)
endif()

target_include_directories(sequencer-obj
    PRIVATE ${LIBXML2_INCLUDE_DIR}
    PRIVATE $ENV{CODAC_ROOT}/include
    )

set_target_properties(sequencer-shared PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    OUTPUT_NAME sequencer
)

target_link_directories(sequencer-shared
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer-shared
    PRIVATE
        ${LIBXML2_LIBRARIES}
    PUBLIC
        sequencer-obj
        ccs-core
    )

set_target_properties(sequencer-static PROPERTIES
    OUTPUT_NAME sequencer
)

target_link_directories(sequencer-static
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer-static
    PRIVATE
        ${LIBXML2_LIBRARIES}
    PUBLIC
        sequencer-obj
    )

target_include_directories(sequencer-cli
    PRIVATE $ENV{CODAC_ROOT}/include
    )

target_link_directories(sequencer-cli
    PRIVATE $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer-cli
    PRIVATE
        sequencer-shared
    )

target_include_directories(sequencer-daemon
    PRIVATE $ENV{CODAC_ROOT}/include
    )

target_link_directories(sequencer-daemon
    PRIVATE $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sequencer-daemon
    PRIVATE
        sequencer-shared
    )

install(TARGETS sequencer-cli DESTINATION bin)
install(TARGETS sequencer-daemon DESTINATION bin)
install(TARGETS sequencer-shared DESTINATION lib)
install(TARGETS sequencer-static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include
    FILES_MATCHING PATTERN "*.h")