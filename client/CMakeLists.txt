cmake_minimum_required(VERSION 3.16)

project(client_IV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick) 
#find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_add_executable(appclient_IV
    main.cpp
)

qt_add_qml_module(appclient_IV
    URI client_IV
    VERSION 1.0
    QML_FILES main.qml 
    VP_Button.qml
    VP_TextEdit.qml
)

set_target_properties(appclient_IV PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appclient_IV
    PRIVATE Qt6::Quick)

install(TARGETS appclient_IV
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)
