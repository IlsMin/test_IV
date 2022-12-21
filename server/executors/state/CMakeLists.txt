cmake_minimum_required(VERSION 3.5)

project(state LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

include_directories ("../../")

add_library(state SHARED
  ../../executorInterface.h
  state_global.h
  state.cpp
  state.h
)

target_link_libraries(state PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(state PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(state PRIVATE STATE_LIBRARY)
