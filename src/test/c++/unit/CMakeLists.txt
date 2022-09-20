add_executable(unit-tests)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/log_ui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/attribute_map_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/choice_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cl_interface_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/compound_instruction_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/condition_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/daemon_interface_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/decorator_instruction_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/equals_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/execution_status_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fallback_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/file_variable_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/input_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/instruction_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/instruction_registry_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/listen_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/local_variable_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/message_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/named_callback_manager_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/output_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/parallel_sequence_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/procedure_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/procedure_to_treedata_utils_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/register_type_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/repeat_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/reset_variable_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/runner_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/sequence_parser_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/sequence_workspace_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/treedata_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/treedata_xml_write_utils_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/unit_test_helper.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/user_choice_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/user_interface_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/variable_registry_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/wait_tests.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/workspace_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/sup/sequencer-cli/cl_interface.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/sup/sequencer-daemon/daemon_interface.cpp
)

target_include_directories(unit-tests
    PRIVATE
        $ENV{CODAC_ROOT}/include
)

target_link_directories(unit-tests
    PRIVATE
        $ENV{CODAC_ROOT}/lib
)

find_package(GTest REQUIRED)

target_link_libraries(unit-tests
    PRIVATE
        GTest::GTest
        GTest::Main
        gmock
        sup-sequencer-shared
)
