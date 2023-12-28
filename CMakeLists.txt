cmake_minimum_required(VERSION 3.5)

project(YourHUDs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Add your sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    hud.h
    hud.cpp
    resource.qrc
    unzip.py  # Add the Python script to the sources
)

# Copy Python script to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/unzip.py ${CMAKE_CURRENT_BINARY_DIR}/unzip.py COPYONLY)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(YourHUDs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        customhud.h customhud.cpp
        customwindow.h customwindow.cpp customwindow.ui

    )
else()
    if(ANDROID)
        add_library(YourHUDs SHARED
            ${PRgOJECT_SOURCES}
        )
    else()
        add_executable(YourHUDs
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link against Miniz library
target_link_libraries(YourHUDs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Set target properties and install
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.YourHUDs)
endif()

set_target_properties(YourHUDs PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS YourHUDs
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(YourHUDs)
endif()
