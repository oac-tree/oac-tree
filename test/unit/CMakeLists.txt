add_executable(unit-tests)

target_compile_features(unit-tests PRIVATE cxx_std_11)

target_sources(unit-tests
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/attribute_map_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/choice_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cl_interface_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comparison_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compound_instruction_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/condition_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/copy_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/daemon_interface_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/decorator_instruction_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/decrement_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/equals_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/execution_status_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fallback_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file_variable_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/for_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/generic_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/input_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/increment_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instruction_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instruction_registry_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/listen_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/log_severity_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/local_variable_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/message_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/named_callback_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/output_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parallel_sequence_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/procedure_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/procedure_to_treedata_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/register_type_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/repeat_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/reset_variable_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runner_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/sequence_parser_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/sequence_workspace_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/unit_test_helper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/user_choice_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/user_interface_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/variable_registry_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/wait_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/waitforvariable_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/workspace_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/app/sequencer-cli/cl_interface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/app/sequencer-daemon/daemon_interface.cpp
)

target_include_directories(unit-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../src)

find_package(GTest REQUIRED)
include(GoogleTest)

if (GTest_VERSION VERSION_LESS_EQUAL "1.8")
  target_link_libraries(unit-tests PRIVATE pthread gtest gtest_main gmock sup-sequencer-shared)
else()
  target_link_libraries(unit-tests PRIVATE pthread GTest::gtest GTest::gtest_main GTest::gmock sup-sequencer-shared)
endif()

find_package(sup-utils REQUIRED)
target_link_libraries(unit-tests PRIVATE sup-utils::sup-log sup-utils::sup-xml)

set_target_properties(unit-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(unit-tests)
