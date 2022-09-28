find_package(LibXml2 REQUIRED)

add_library(sup-sequencer-shared SHARED)

add_subdirectory(base)
add_subdirectory(instructions)
add_subdirectory(log)
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

target_include_directories(sup-sequencer-shared PRIVATE PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../..)
target_link_libraries(sup-sequencer-shared PRIVATE LibXml2::LibXml2)

if (SEQUENCER_CODAC)
  target_include_directories(sup-sequencer-shared PRIVATE $ENV{CODAC_ROOT}/include)
  target_link_directories(sup-sequencer-shared PUBLIC $ENV{CODAC_ROOT}/lib)
  target_link_libraries(sup-sequencer-shared PUBLIC sup-dto)
else()
  find_package(sup-dto REQUIRED)
  target_link_libraries(sup-sequencer-shared PUBLIC sup-dto::sup-dto)
endif()
