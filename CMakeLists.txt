cmake_minimum_required(VERSION 4.2.3)
project(project-sillyfied-installer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB SOURCES "projsilinstaller/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/projsilinstaller/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    CURL::libcurl
    ZLIB::ZLIB
)
