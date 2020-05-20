cmake_minimum_required(VERSION 3.0)

project("AGE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

# freetype path
if(APPLE)
    include_directories("/usr/local/Cellar/freetype/2.10.1/include/freetype2")
endif(APPLE)

add_subdirectory(AGE)
