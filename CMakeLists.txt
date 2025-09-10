cmake_minimum_required(VERSION 3.16)

project(UltimateColorPicker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES ts_files/UltimateColorPicker_ru_RU.ts)

option(BUILD_TESTS "Build tests" OFF)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        include/mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UltimateColorPicker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        src/AddIcon.cpp src/ColorCell.cpp src/ColorCombo.cpp src/ColoredEllipse.cpp src/ColorInfo.cpp src/ColorPallete.cpp src/ColorPicker.cpp src/ColorWheel.cpp src/DeletePopup.cpp src/GridLayout.cpp src/main.cpp src/mainwindow.cpp src/NamedPallete.cpp src/NavigationBar.cpp src/ShadeCircle.cpp src/Utils.cpp
        include/AddIcon.hpp include/ColorCell.hpp include/ColorCombo.hpp include/ColoredEllipse.hpp include/ColorInfo.hpp include/ColorPallete.hpp include/ColorPicker.hpp include/ColorWheel.hpp include/DeletePopup.hpp include/GridLayout.hpp  include/mainwindow.h include/NamedPallete.hpp include/NavigationBar.hpp include/ShadeCircle.hpp include/Utils.hpp
        application.qrc
        include/ColorConversion.hpp
        src/ColorConversion.cpp
    )

    target_include_directories(UltimateColorPicker PRIVATE  include)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET UltimateColorPicker APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(UltimateColorPicker SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(UltimateColorPicker
            ${PROJECT_SOURCES}
        )

        target_include_directories(UltimateColorPicker PRIVATE  include)
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(UltimateColorPicker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_custom_command(TARGET UltimateColorPicker PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/configs $<TARGET_FILE_DIR:UltimateColorPicker>/data)

if(MSVC)
  target_compile_options(UltimateColorPicker PRIVATE /W4 /WX)
else()
    set(CompileOptions  "-Wall;-Wextra;-Wpedantic;" )

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        list(APPEND CompileOptions "-Werror" )
        message(${CompileOptions})


    endif()
        message(${CompileOptions})
        target_compile_options(UltimateColorPicker PRIVATE ${CompileOptions})
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.UltimateColorPicker)
endif()
set_target_properties(UltimateColorPicker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

include(GNUInstallDirs)

install(TARGETS UltimateColorPicker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UltimateColorPicker)
endif()
