add_library(sup-oac-tree-shared SHARED)
add_library(oac-tree::oac-tree ALIAS sup-oac-tree-shared) # alias for build-tree usage

add_subdirectory(attributes)
add_subdirectory(base)
add_subdirectory(instructions)
add_subdirectory(parser)
add_subdirectory(procedure)
add_subdirectory(runner)
add_subdirectory(variables)

set_target_properties(sup-oac-tree-shared PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    OUTPUT_NAME oac-tree
)

target_include_directories(sup-oac-tree-shared PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>)

target_link_libraries(sup-oac-tree-shared PUBLIC sup-dto::sup-dto PRIVATE sup-utils::sup-xml)

install(TARGETS sup-oac-tree-shared EXPORT oac-tree-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(sup-oac-tree-shared PROPERTIES EXPORT_NAME oac-tree)

install(FILES
  anyvalue_utils.h
  application_utils.h
  async_input_adapter.h
  async_runner.h
  attribute_definition.h
  attribute_handler.h
  attribute_info.h
  attribute_utils.h
  breakpoint.h
  compound_instruction.h
  concrete_constraints.h
  constants.h
  constraint.h
  decorator_instruction.h
  exceptions.h
  execution_status.h
  generic_utils.h
  i_job_info_io.h
  i_job.h
  i_user_input_future.h
  instruction_info_utils.h
  instruction_info.h
  instruction_map.h
  instruction_registry.h
  instruction_state.h
  instruction_tree.h
  instruction_utils.h
  instruction.h
  job_command_queue.h
  job_commands.h
  job_info_utils.h
  job_info.h
  job_interface_adapter.h
  job_interface.h
  job_map.h
  job_state_monitor.h
  job_states.h
  local_job.h
  log_severity.h
  named_callback_manager.h
  procedure_context.h
  procedure_preamble.h
  procedure.h
  runner.h
  scope_guard.h
  sequence_parser.h
  setup_teardown_actions.h
  user_input_reply.h
  user_input_request.h
  user_interface.h
  variable_info.h
  variable_map.h
  variable_registry.h
  variable_utils.h
  variable.h
  workspace_info.h
  workspace.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/oac-tree
)
