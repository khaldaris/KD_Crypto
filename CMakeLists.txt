cmake_minimum_required(VERSION 3.5)

project(KD_Crypto LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file (GLOB_RECURSE HEADERS "*.h")
aux_source_directory(./src SRC_LIST)
SET(SRC_LIST ${SRC_LIST} ${HEADERS})

MESSAGE(STSTUS "###################################")
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
MESSAGE(STATUS "current platform: Linux ")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release/Linux)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
MESSAGE(STATUS "current platform: Windows")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release/Windows)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
MESSAGE(STSTUS "###################################")

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)
add_executable(KD_Crypto WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    logo.rc
    Resources.qrc
    ${SRC_LIST}
    )
target_link_libraries(KD_Crypto PRIVATE Qt5::Widgets)
