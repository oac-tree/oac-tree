add_executable(testapp)

target_compile_features(testapp PRIVATE cxx_std_11)

target_sources(testapp
  PRIVATE
  main.cpp
  test_procedure_factory.cpp
  test_ui.cpp
)

target_link_libraries(testapp PRIVATE sup-sequencer-shared)

set_target_properties(testapp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
