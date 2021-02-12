add_executable(unit-tests)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParser-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceWorkspace-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Condition-tests.cpp
)

target_include_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/include
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/instructions
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/parser
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/procedure
        # ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/runner
        $ENV{CODAC_ROOT}/include
)

target_link_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../target/lib
        $ENV{CODAC_ROOT}/lib
)

find_package(GTest REQUIRED)

target_link_libraries(unit-tests sequencer GTest::GTest GTest::Main ccs-types ccs-base ccs-common MathExpressionEngineExptrk)
