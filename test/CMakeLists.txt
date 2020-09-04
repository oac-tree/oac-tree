add_executable(Test
    main.cpp
)

target_sources(SequenceRunner
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SuccesNode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TestProcedureFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TestUI.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/SuccesNode.h
        ${CMAKE_CURRENT_LIST_DIR}/TestProcedureFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/TestUI.h
)

target_link_libraries(Test SequenceRunner)
