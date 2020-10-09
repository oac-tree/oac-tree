cmake_minimum_required(VERSION 3.5)

project(Sequencer-unit-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(unit-tests
    main.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../main/c++
)

target_link_directories(unit-tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/SuccesNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProcedureFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestUI.cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/SuccesNode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProcedureFactory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TestUI.h
)

target_link_libraries(unit-tests sequencer)
