add_library(MathExpressionEngineExptrk)

set_target_properties(MathExpressionEngineExptrk PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
)

target_sources(MathExpressionEngineExptrk
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/exprtk.hpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineExptrk.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineProvider.cpp
    )

target_include_directories(MathExpressionEngineExptrk
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../../include
        $ENV{CODAC_ROOT}/include
    )

target_compile_definitions(MathExpressionEngineExptrk
    PRIVATE
        exprtk_disable_caseinsensitivity
        exprtk_disable_rtl_vecops
    )