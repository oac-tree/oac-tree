add_library(sup-sequencer-shared SHARED)
add_library(sequencer::sequencer ALIAS sup-sequencer-shared) # alias for build-tree usage

add_subdirectory(attributes)
add_subdirectory(base)
add_subdirectory(instructions)
add_subdirectory(parser)
add_subdirectory(procedure)
add_subdirectory(runner)
add_subdirectory(variables)

set_target_properties(sup-sequencer-shared PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    OUTPUT_NAME sequencer
)

target_include_directories(sup-sequencer-shared PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>)

target_link_libraries(sup-sequencer-shared PUBLIC sup-dto::sup-dto PRIVATE sup-utils::sup-xml)

install(TARGETS sup-sequencer-shared EXPORT sequencer-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(sup-sequencer-shared PROPERTIES EXPORT_NAME sequencer)

install(FILES
  application_utils.h
  async_runner.h
  attribute_definition.h
  attribute_handler.h
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
  i_job.h
  instruction_registry.h
  instruction_tree.h
  instruction_utils.h
  instruction.h
  job_command_queue.h
  job_commands.h
  job_interface.h
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
  user_interface.h
  variable_registry.h
  variable.h
  workspace.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/sequencer
)
