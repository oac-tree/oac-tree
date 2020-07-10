find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(SequenceRunner)

add_subdirectory(runner)
add_subdirectory(workspace)

target_include_directories (SequenceRunner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
