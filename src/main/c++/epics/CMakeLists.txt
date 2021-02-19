add_library(seq-epics SHARED)

set_target_properties(seq-epics PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
)

target_sources(seq-epics
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ChannelAccessClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/BlockingRPCClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/PVAccessMonitor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LogVariable.cpp
    )

target_include_directories(seq-epics
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../instructions
        ${CMAKE_CURRENT_LIST_DIR}/../variables
        ${CMAKE_CURRENT_LIST_DIR}/../procedure
        $ENV{CODAC_ROOT}/include
## ChannelAccess support now through static inline wrappers requires EPICS header files
        $ENV{CODAC_ROOT}/epics/include
        $ENV{CODAC_ROOT}/epics/include/os/Linux
        $ENV{CODAC_ROOT}/epics/include/compiler/gcc
    )

target_link_directories(seq-epics
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(seq-epics
    PUBLIC
        ccs-common
        ccs-types
        ccs-base
        ccs-ca
        ccs-pvxs
        sequencer-shared
    )

target_compile_definitions(seq-epics
    PRIVATE
        LOG_DEBUG_ENABLE
    )

install(TARGETS seq-epics DESTINATION lib)