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

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)
add_executable(KD_Crypto WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    ${SRC_LIST}
    )
target_link_libraries(KD_Crypto PRIVATE Qt5::Widgets)
