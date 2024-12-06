cmake_minimum_required(VERSION 3.16)

project(exphone LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(FeatureSummary)

find_package(Qt5 COMPONENTS Core Gui Quick Sql DBus LinguistTools REQUIRED)

#set(WATCHFISH_FEATURES "notificationmonitor;walltime;music;calendar;voicecall;volume")
set(WATCHFISH_FEATURES "voicecall")

if(NOT DEFINED FLAVOR)
    message(FATAL_ERROR "Please specify the platform using -DFLAVOR=platform as a CMake option. Supported platforms: kirigami, silica, qtcontrols, uuitk.")
elseif(FLAVOR STREQUAL "silica")
    add_definitions(-DMER_EDITION_SAILFISH)
#elseif(FLAVOR STREQUAL "kirigami")
#    add_definitions(-Dflavor_kirigami)
#elseif(FLAVOR STREQUAL "qtcontrols")
#    add_definitions(-Dflavor_qtcontrols)
elseif(FLAVOR STREQUAL "uuitk")
    add_definitions(-DUUITK_EDITION)
else()
    message(FATAL_ERROR "Unsupported platform '${FLAVOR}'. Supported platforms: kirigami, silica, qtcontrols, uuitk.")
endif()

add_subdirectory(libwatchfish)
add_subdirectory(lib)
add_subdirectory(daemon)
add_subdirectory(ui)
