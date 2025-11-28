cmake_minimum_required(VERSION 3.13)
project(Silk)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(Silk
    src/main.cpp
    src/browserframe.cpp
    src/navigationbar.cpp
    src/defaulttab.cpp
)

target_link_libraries(Silk
    Qt5::Widgets
    Qt5::WebEngineWidgets
)

