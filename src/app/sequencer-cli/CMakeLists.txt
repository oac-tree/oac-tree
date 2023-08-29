add_executable(sequencer-cli)

target_compile_features(sequencer-cli PRIVATE cxx_std_11)

target_sources(sequencer-cli
  PRIVATE
  cl_interface.cpp
  main.cpp
)

target_link_libraries(sequencer-cli PRIVATE
  sup-sequencer-shared
  sup-utils::sup-log
  sup-utils::sup-cli
)

install(TARGETS sequencer-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
