find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Sequencer
    MainWindow.cpp
    MainWindow.h
    TestModel.cpp
    TestModel.h
    main.cpp
)

add_subdirectory(model)
add_subdirectory(viewmodel)

target_include_directories (Sequencer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Sequencer Qt5::Widgets SequenceRunner)
