cmake_minimum_required(VERSION 3.15)
project(downloader)

set(CMAKE_CXX_STANDARD 17)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

message(STATUS "Qt found in ${CONAN_QT_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(SRC_FILES
        src/main.cpp
        src/DownloadController.cpp
        src/Downloader/Writer.cpp
        src/Downloader/ReplyWrapper.cpp
        src/Downloader/Downloader.cpp
        src/UI/DownloadDialog.cpp
        )

add_executable(downloader ${SRC_FILES})

target_link_libraries(downloader Qt5::Core Qt5::Widgets Qt5::Network)
