add_executable(unit-tests)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/AttributeMap-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Choice-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Condition-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileVariable-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LocalVariable-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ParallelSequence-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Runner-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceParser-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SequenceWorkspace-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TreeData-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UserInterface-tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/VariableRegistry-tests.cpp
    )

target_include_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/include
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/instructions
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/variables
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/parser
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++/procedure
        $ENV{CODAC_ROOT}/include
    )

target_link_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../../target/lib
        $ENV{CODAC_ROOT}/lib
    )

find_package(GTest REQUIRED)
find_package(PkgConfig)
pkg_check_modules(GMOCK REQUIRED gmock>=1.6.0)
pkg_get_variable(GMOCK_SRC_DIR gmock srcdir)
target_sources(unit-tests
    PRIVATE
        ${GMOCK_SRC_DIR}/gmock-all.cc
    )

target_link_libraries(unit-tests sequencer GTest::GTest GTest::Main ccs-types ccs-base ccs-common)
