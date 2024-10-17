add_executable(unit-tests)

target_sources(unit-tests
  PRIVATE
    add_element_tests.cpp
    add_member_tests.cpp
    async_runner_tests.cpp
    attribute_handler_tests.cpp
    attribute_utils_tests.cpp
    choice_tests.cpp
    cl_interface_tests.cpp
    comparison_tests.cpp
    compound_instruction_tests.cpp
    condition_tests.cpp
    constraint_tests.cpp
    copy_from_procedure_tests.cpp
    copy_tests.cpp
    copy_to_procedure_tests.cpp
    daemon_interface_tests.cpp
    decorator_instruction_tests.cpp
    decrement_tests.cpp
    equals_tests.cpp
    execution_status_tests.cpp
    fallback_tests.cpp
    file_variable_tests.cpp
    for_tests.cpp
    generic_utils_tests.cpp
    include_procedure_tests.cpp
    include_tests.cpp
    increment_tests.cpp
    input_tests.cpp
    instruction_map_tests.cpp
    instruction_registry_tests.cpp
    instruction_tests.cpp
    instruction_tree_tests.cpp
    job_command_queue_tests.cpp
    job_command_tests.cpp
    listen_tests.cpp
    log_instruction_tests.cpp
    log_severity_tests.cpp
    local_variable_tests.cpp
    message_tests.cpp
    named_callback_manager_tests.cpp
    nested_listen_tests.cpp
    output_tests.cpp
    parallel_sequence_tests.cpp
    procedure_tests.cpp
    procedure_to_treedata_utils_tests.cpp
    register_type_tests.cpp
    repeat_tests.cpp
    reset_variable_tests.cpp
    runner_tests.cpp
    scope_guard_tests.cpp
    sequence_parser_tests.cpp
    sequence_workspace_tests.cpp
    unit_test_helper.cpp
    user_choice_tests.cpp
    user_confirmation_tests.cpp
    user_interface_tests.cpp
    var_exists_tests.cpp
    variable_info_tests.cpp
    variable_registry_tests.cpp
    wait_tests.cpp
    wait_for_variable_tests.cpp
    wait_for_variables_tests.cpp
    workspace_info_tests.cpp
    workspace_tests.cpp
    ../../src/app/sequencer-cli/cl_interface.cpp
    ../../src/app/sequencer-daemon/daemon_interface.cpp
)

target_include_directories(unit-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../src)

target_link_libraries(unit-tests
  PRIVATE
  sup-sequencer-shared
  sup-utils::sup-log
  sup-utils::sup-xml
  pthread
)

if(GTest_VERSION VERSION_LESS_EQUAL "1.8")
  target_link_libraries(unit-tests PRIVATE gtest gtest_main gmock)
else()
  target_link_libraries(unit-tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock)
endif()

set_target_properties(unit-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(unit-tests)
