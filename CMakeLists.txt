cmake_minimum_required(VERSION 3.15)
project(downloader)

set(CMAKE_CXX_STANDARD 11)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(SEND_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

message(STATUS "Qt found in ${CONAN_QT_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

add_executable(downloader main.cpp)

target_link_libraries(downloader Qt5::Core Qt5::Widgets Qt5::Network)