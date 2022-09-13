add_executable(testapp)

target_sources(testapp
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TestProcedureFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TestUI.cpp
)

target_include_directories(testapp
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++
        $ENV{CODAC_ROOT}/include
)

target_link_directories(testapp
    PRIVATE
        $ENV{CODAC_ROOT}/lib
)

target_link_libraries(testapp
    PRIVATE
        sup-sequencer-shared
        ccs-common
)
