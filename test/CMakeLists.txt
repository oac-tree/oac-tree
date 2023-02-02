set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(unit)
add_subdirectory(testapp)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
TEST_RESOURCES_PATH=" ${CMAKE_CURRENT_LIST_DIR}/resources " " ${TEST_OUTPUT_DIRECTORY} "/unit-tests \"$@\"
exit $?
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
