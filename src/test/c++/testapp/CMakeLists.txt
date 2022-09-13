set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(testapp
    main.cpp
)

target_include_directories(testapp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../main/c++
        $ENV{CODAC_ROOT}/include
)

target_link_directories(testapp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        $ENV{CODAC_ROOT}/lib
)

target_sources(testapp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProcedureFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestUI.cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProcedureFactory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TestUI.h
)

target_link_libraries(testapp sequencer-shared ccs-common)
