add_library(MathExpression SHARED)

set_target_properties(MathExpression PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
)

target_sources(MathExpression
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/exprtk.hpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineExptrk.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineProvider.cpp
    )

target_include_directories(MathExpression
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        $ENV{CODAC_ROOT}/include
    )

target_compile_definitions(MathExpression
    PRIVATE
        exprtk_disable_caseinsensitivity
        exprtk_disable_rtl_vecops
    )

target_link_libraries(MathExpression
    PRIVATE
        sequencer-shared
    )

install(TARGETS MathExpression DESTINATION lib/plugins)