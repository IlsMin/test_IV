cmake_minimum_required(VERSION 3.14)

project(rate LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DRATELIBRARY)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

include_directories ("../../")

add_library(rate SHARED

  ../../executorInterface.h
  rate_global.h
  rate.h
  rate.cpp
)

target_link_libraries(rate PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(rate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(rate PRIVATE RATE_LIBRARY)

