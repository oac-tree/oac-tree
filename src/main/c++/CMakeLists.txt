cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEFINED COVERAGE AND ${COVERAGE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(sequencer-cli)

add_executable(sequencer-daemon)

add_library(sup-sequencer-obj OBJECT)
set_property(TARGET sup-sequencer-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(sup-sequencer-shared SHARED)
add_library(sup-sequencer-static STATIC)

find_package(LibXml2 REQUIRED)

add_subdirectory(sup)

target_include_directories(sup-sequencer-obj
    PRIVATE
        ${LIBXML2_INCLUDE_DIR}
        $ENV{CODAC_ROOT}/include
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(sup-sequencer-shared PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    OUTPUT_NAME sequencer
)

target_link_directories(sup-sequencer-shared
    PUBLIC $ENV{CODAC_ROOT}/lib
)

target_link_libraries(sup-sequencer-shared
    PRIVATE
        ${LIBXML2_LIBRARIES}
    PUBLIC
        sup-sequencer-obj
        ccs-core
)

set_target_properties(sup-sequencer-static PROPERTIES
    OUTPUT_NAME sequencer
)

target_link_directories(sup-sequencer-static
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(sup-sequencer-static
    PRIVATE
        ${LIBXML2_LIBRARIES}
    PUBLIC
        sup-sequencer-obj
)

target_include_directories(sequencer-cli
    PRIVATE
        $ENV{CODAC_ROOT}/include
)

target_link_libraries(sequencer-cli
    PRIVATE
        sup-sequencer-shared
)

target_include_directories(sequencer-daemon
    PRIVATE
        $ENV{CODAC_ROOT}/include
)

target_link_libraries(sequencer-daemon
    PRIVATE
        sup-sequencer-shared
)

install(FILES
        sup/sequencer/AttributeMap.h
        sup/sequencer/Constants.h
        sup/sequencer/ExecutionStatus.h
        sup/sequencer/Instruction.h
        sup/sequencer/InstructionRegistry.h
        sup/sequencer/log.h
        sup/sequencer/NamedCallbackManager.h
        sup/sequencer/Procedure.h
        sup/sequencer/Runner.h
        sup/sequencer/SequenceParser.h
        sup/sequencer/TreeData.h
        sup/sequencer/UserInterface.h
        sup/sequencer/Variable.h
        sup/sequencer/VariableRegistry.h
        sup/sequencer/Workspace.h
        DESTINATION include/sup/sequencer
)