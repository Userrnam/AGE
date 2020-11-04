cmake_minimum_required(VERSION 3.10)

project("AGE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_subdirectory(AGE)
# add_subdirectory(Examples)
