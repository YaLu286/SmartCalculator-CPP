cmake_minimum_required(VERSION 3.5)

project(smart_calc VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INSTALL_PREFIX .)
# set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/6.6.0/lib/cmake/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        ./qcustomplot.h
        ./qcustomplot.cc
        ../model/polish.h
        ../model/polish.cc
        ../controller/calc_controller.h
        ../controller/calc_controller.cc
        ./mainwindow.h
        ./mainwindow.cc
        ./mainwindow.ui
        ./main.cc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(smart_calc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET smart_calc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(smart_calc SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(smart_calc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(smart_calc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.smart_calc)
endif()
set_target_properties(smart_calc PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS smart_calc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(smart_calc)
endif()
