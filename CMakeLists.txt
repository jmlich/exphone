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
set(FLAVOR "kirigami") # FIXME

add_subdirectory(libwatchfish)
add_subdirectory(lib)
add_subdirectory(daemon)
add_subdirectory(ui)
