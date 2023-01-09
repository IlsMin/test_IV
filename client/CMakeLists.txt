cmake_minimum_required(VERSION 3.16)

project(client_IV VERSION 0.1 LANGUAGES CXX)

#message( ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
message(${CMAKE_PREFIX_PATH})
#message( ${QT_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build_client/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

find_package(Qt6 REQUIRED COMPONENTS Quick) 
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)

qt_add_executable(client_IV
    main.cpp
    clientsocket.h
    clientsocket.cpp
)

qt_add_qml_module(client_IV
    URI client_IV
    VERSION 1.0
    QML_FILES main.qml 
    VP_Button.qml
    VP_TextEdit.qml
)

set_target_properties(client_IV PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(client_IV PRIVATE Qt6::Quick)
target_link_libraries(client_IV PRIVATE Qt6::Qml)

install(TARGETS client_IV
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )
