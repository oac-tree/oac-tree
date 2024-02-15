if(NOT COA_BUILD_TESTS)
  return()
endif()

find_package(GTest 1.10 REQUIRED)
include(GoogleTest)

add_subdirectory(unit)
add_subdirectory(parasoft)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
TEST_RESOURCES_PATH=" ${CMAKE_CURRENT_LIST_DIR}/resources " " ${TEST_OUTPUT_DIRECTORY} "/unit-tests \"$@\"
exit $?
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
