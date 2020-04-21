cmake_minimum_required(VERSION 3.5)

project(Sequencer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Sequencer
    MainWindow.cpp
    MainWindow.h
    TestModel.cpp
    TestModel.h
    main.cpp
)

add_subdirectory(model)
add_subdirectory(viewmodel)

include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(Sequencer Qt5::Widgets)
