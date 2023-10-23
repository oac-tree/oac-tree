#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if(COA_COVERAGE)
  message(STATUS "Enabling test coverage information")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-inline --coverage")
endif()

add_subdirectory(lib)
add_subdirectory(app)
