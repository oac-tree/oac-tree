add_library(MathExpression)

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
        ${CMAKE_CURRENT_LIST_DIR}/../../include
        $ENV{CODAC_ROOT}/include
    )

target_compile_definitions(MathExpression
    PRIVATE
        exprtk_disable_caseinsensitivity
        exprtk_disable_rtl_vecops
    )