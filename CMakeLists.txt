cmake_minimum_required(VERSION 3.15)
project(downloader)

set(CMAKE_CXX_STANDARD 11)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(SEND_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_executable(downloader main.cpp)