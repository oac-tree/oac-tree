add_executable(unit-tests)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParser-tests.cpp
)

target_include_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../main/c++
        $ENV{CODAC_ROOT}/include
)

target_link_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib
        $ENV{CODAC_ROOT}/lib
)

find_package(GTest REQUIRED)

target_link_libraries(unit-tests sequencer GTest::GTest GTest::Main ccs-types ccs-base ccs-common)
