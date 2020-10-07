add_library(SequenceRunner)

add_subdirectory(runner)
add_subdirectory(procedure)

target_include_directories (SequenceRunner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
