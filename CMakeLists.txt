cmake_minimum_required(VERSION 3.14)
project(diary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets UiTools Sql)
qt_standard_project_setup()

add_executable(diary
   src/main_window.cpp
   src/main.cpp
   )

target_link_libraries(diary Qt6::Widgets Qt6::UiTools Qt6::Sql)