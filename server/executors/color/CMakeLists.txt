cmake_minimum_required(VERSION 3.14)

project(color LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(QT_USE_QTPLUGIN TRUE)
#set(TPLUGIN_LIBRARY ON)
#set(COLOR_LIBRARY)

#add_definitions(COLOR_LIBRARY)

add_definitions(-DCOLOR_LIBRARY)
#add_compile_definitions(COLOR_LIBRARY)
#target_compile_definitions(COLOR_LIBRARY=1)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories ("../../")

add_library(color SHARED

  ../../executorInterface.h
  color_global.h
  color.h
  color.cpp
)

target_link_libraries(color PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(color PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(color PRIVATE RATE_LIBRARY)
