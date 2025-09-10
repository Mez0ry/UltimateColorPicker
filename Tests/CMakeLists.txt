cmake_minimum_required(VERSION 3.16)

project(Tests LANGUAGES CXX)

enable_testing()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ($ENV{CATCH2_INSTALL_DIR})
    get_filename_component(_CATCH2_BASE $ENV{CATCH2_INSTALL_DIR} REALPATH)
elseif (EXISTS "") # set by QC
    get_filename_component(_CATCH2_BASE "" REALPATH) # set by QC
endif()

if (NOT Catch2_DIR)
    if (_CATCH2_BASE)
        if (EXISTS "${_CATCH2_BASE}/lib/cmake/Catch2")
            set(_CATCH2_BASE "${_CATCH2_BASE}/lib/cmake/Catch2")
        endif()
        message("Setting Catch2_DIR to ${_CATCH2_BASE}")
        set(Catch2_DIR ${_CATCH2_BASE})
    else()
        message("No Catch2_DIR specified - using system defaults.")
    endif()
endif()

find_package(Catch2 3 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

add_executable(Tests TestColorConversion.cpp
    main.cpp ${CMAKE_SOURCE_DIR}/src/ColorConversion.cpp
    IsBetweenMatcher.hpp)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(Tests PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME Tests COMMAND Tests)
