cmake_minimum_required(VERSION 3.19)
project(StudentApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/Student.h
    src/StudentModel.h
    src/StudentModel.cpp
    src/StudentProxy.h
    src/StudentProxy.cpp
    src/GenderBadgeDelegate.h
    src/GenderBadgeDelegate.cpp
    src/StudentDialog.h
    src/StudentDialog.cpp
    src/MainWindow.h
    src/MainWindow.cpp
)

qt_add_executable(StudentApp MANUAL_FINALIZATION ${PROJECT_SOURCES} resources/resources.qrc)

target_link_libraries(StudentApp PRIVATE Qt6::Widgets)

target_include_directories(StudentApp PRIVATE src)

set_target_properties(StudentApp PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(StudentApp)
