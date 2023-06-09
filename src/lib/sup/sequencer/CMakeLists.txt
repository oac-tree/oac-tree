add_library(sup-sequencer-shared SHARED)
add_library(sequencer::sequencer ALIAS sup-sequencer-shared) # alias for build-tree usage

target_compile_features(sup-sequencer-shared PRIVATE cxx_std_11)

add_subdirectory(attributes)
add_subdirectory(base)
add_subdirectory(instructions)
add_subdirectory(parser)
add_subdirectory(procedure)
add_subdirectory(runner)
add_subdirectory(variables)

if(NOT DEFINED ENV{NO_PLUGINS})
    add_subdirectory(plugins)
endif()

set_target_properties(sup-sequencer-shared PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    OUTPUT_NAME sequencer
)

target_include_directories(sup-sequencer-shared PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>)

find_package(sup-dto REQUIRED)
find_package(sup-utils REQUIRED)
target_link_libraries(sup-sequencer-shared PUBLIC sup-dto::sup-dto PRIVATE sup-utils::sup-xml)

install(TARGETS sup-sequencer-shared EXPORT sequencer-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(sup-sequencer-shared PROPERTIES EXPORT_NAME sequencer)

install(FILES
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
  instruction_registry.h
  instruction_tree.h
  instruction.h
  log_severity.h
  named_callback_manager.h
  procedure.h
  runner.h
  scope_guard.h
  sequence_parser.h
  user_interface.h
  variable_registry.h
  variable.h
  workspace.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/sequencer
)
