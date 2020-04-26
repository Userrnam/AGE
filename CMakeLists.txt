cmake_minimum_required(VERSION 3.0)

project("AGE")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(AGE)
