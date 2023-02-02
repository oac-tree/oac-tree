if (DEFINED COVERAGE AND ${COVERAGE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
endif()

add_subdirectory(lib)
add_subdirectory(app)
